#include "hermes2d.h"
#include "solver_umfpack.h"

//  This test makes sure that the benchmark "singpert" works correctly.

const int INIT_REF_NUM = 1;       // number of initial mesh refinements (the original mesh is just one element)
const int INIT_REF_NUM_BDY = 0;   // number of initial mesh refinements towards the boundary
const int P_INIT = 1;             // Initial polynomial degree of all mesh elements.
const double THRESHOLD = 0.3;     // This is a quantitative parameter of the adapt(...) function and
                                  // it has different meanings for various adaptive strategies (see below).
const int STRATEGY = 0;           // Adaptive strategy:
                                  // STRATEGY = 0 ... refine elements until sqrt(THRESHOLD) times total
                                  //   error is processed. If more elements have similar errors, refine
                                  //   all to keep the mesh symmetric.
                                  // STRATEGY = 1 ... refine all elements whose error is larger
                                  //   than THRESHOLD times maximum element error.
                                  // STRATEGY = 2 ... refine all elements whose error is larger
                                  //   than THRESHOLD.
                                  // More adaptive strategies can be created in adapt_ortho_h1.cpp.
const int ADAPT_TYPE = 0;         // Type of automatic adaptivity:
                                  // ADAPT_TYPE = 0 ... adaptive hp-FEM (default),
                                  // ADAPT_TYPE = 1 ... adaptive h-FEM,
                                  // ADAPT_TYPE = 2 ... adaptive p-FEM.
const bool ISO_ONLY = false;      // Isotropic refinement flag (concerns quadrilateral elements only).
                                  // ISO_ONLY = false ... anisotropic refinement of quad elements
                                  // is allowed (default),
                                  // ISO_ONLY = true ... only isotropic refinements of quad elements
                                  // are allowed.
const int MESH_REGULARITY = -1;   // Maximum allowed level of hanging nodes:
                                  // MESH_REGULARITY = -1 ... arbitrary level hangning nodes (default),
                                  // MESH_REGULARITY = 1 ... at most one-level hanging nodes,
                                  // MESH_REGULARITY = 2 ... at most two-level hanging nodes, etc.
                                  // Note that regular meshes are not supported, this is due to
                                  // their notoriously bad performance.
const double ERR_STOP = 0.01;     // Stopping criterion for adaptivity (rel. error tolerance between the
                                  // fine mesh and coarse mesh solution in percent).
const int NDOF_STOP = 100000;     // Adaptivity process stops when the number of degrees of freedom grows
                                  // over this limit. This is to prevent h-adaptivity to go on forever.

// problem constants
const double K_squared = 1e4;     // Equation parameter.
const double CONST_F = 1e4;       // Constant right-hand side (set to be roughly K*K for scaling purposes).

// boundary condition types
int bc_types(int marker)
{
  return BC_ESSENTIAL;
}

// function values for Dirichlet boundary conditions
scalar bc_values(int marker, double x, double y)
{
  return 0;
}

template<typename Real, typename Scalar>
Scalar bilinear_form(int n, double *wt, Func<Real> *u, Func<Real> *v, Geom<Real> *e, ExtData<Scalar> *ext)
{
  return int_grad_u_grad_v<Real, Scalar>(n, wt, u, v) + K_squared * int_u_v<Real, Scalar>(n, wt, u, v);
}

template<typename Real, typename Scalar>
Scalar linear_form(int n, double *wt, Func<Real> *v, Geom<Real> *e, ExtData<Scalar> *ext)
{
  return CONST_F * int_v<Real, Scalar>(n, wt, v);
}


int main(int argc, char* argv[])
{
  // load the mesh
  Mesh mesh;
  H2DReader mloader;
  mloader.load("square.mesh", &mesh);

  // initial mesh refinement
  for (int i=0; i < INIT_REF_NUM; i++) mesh.refine_all_elements();
  mesh.refine_towards_boundary(1, INIT_REF_NUM_BDY);

  // initialize the shapeset and the cache
  H1ShapesetOrtho shapeset;
  PrecalcShapeset pss(&shapeset);

  // create finite element space
  H1Space space(&mesh, &shapeset);
  space.set_bc_types(bc_types);
  space.set_bc_values(bc_values);
  space.set_uniform_order(P_INIT);

  // enumerate basis functions
  space.assign_dofs();

  // initialize the weak formulation
  WeakForm wf(1);
  wf.add_biform(0, 0, callback(bilinear_form), SYM);
  wf.add_liform(0, callback(linear_form));

  // matrix solver
  UmfpackSolver solver;

  // convergence graph wrt. the number of degrees of freedom
  GnuplotGraph graph;
  graph.set_captions("Error Convergence for the Singularly Perturbed Problem", "Degrees of Freedom", "Error Estimate [%]");
  graph.add_row("error estimate", "k", "--");
  graph.set_log_y();

  // convergence graph wrt. CPU time
  GnuplotGraph graph_cpu;
  graph_cpu.set_captions("Error Convergence for the Singularly Perturbed Problem", "CPU Time", "Error Estimate [%]");
  graph_cpu.add_row("error estimate", "k", "--");
  graph_cpu.set_log_y();

  // adaptivity loop
  int it = 1, ndofs;
  bool done = false;
  double cpu = 0.0;
  Solution sln_coarse, sln_fine;
  do
  {
    info("\n---- Adaptivity step %d ---------------------------------------------\n", it++);

    // time measurement
    begin_time();

    // solve the coarse mesh problem
    LinSystem ls(&wf, &solver);
    ls.set_spaces(1, &space);
    ls.set_pss(1, &pss);
    ls.assemble();
    ls.solve(1, &sln_coarse);

    // time measurement
    cpu += end_time();

    // time measurement
    begin_time();

    // solve the fine mesh problem
    begin_time();
    RefSystem rs(&ls);
    rs.assemble();
    rs.solve(1, &sln_fine);

    // calculate error estimate wrt. fine mesh solution
    H1OrthoHP hp(1, &space);
    double err_est = hp.calc_error(&sln_coarse, &sln_fine) * 100;
    info("Estimate of error: %g%%", err_est);

    // add entry to DOF convergence graph
    graph.add_values(0, space.get_num_dofs(), err_est);
    graph.save("conv_dof.gp");

    // add entry to CPU convergence graph
    graph_cpu.add_values(0, cpu, err_est);
    graph_cpu.save("conv_cpu.gp");

    // if err_est too large, adapt the mesh
    if (err_est < ERR_STOP) done = true;
    else {
      hp.adapt(THRESHOLD, STRATEGY, ADAPT_TYPE, ISO_ONLY, MESH_REGULARITY);
      ndofs = space.assign_dofs();
      if (ndofs >= NDOF_STOP) done = true;
    }

    // time measurement
    cpu += end_time();
  }
  while (done == false);
  verbose("Total running time: %g sec", cpu);

#define ERROR_SUCCESS                               0
#define ERROR_FAILURE                               -1
  int n_dof_allowed = 2000;// ndofs was 3733 at the time this test was created,
                           // but it has to be much less once we allow
                           // different directional degrees in elements
  printf("n_dof_actual = %d\n", ndofs);
  printf("n_dof_allowed = %d\n", n_dof_allowed);
  if (ndofs <= n_dof_allowed) {
    printf("Success!\n");
    return ERROR_SUCCESS;
  }
  else {
    printf("Failure!\n");
    return ERROR_FAILURE;
  }
}
