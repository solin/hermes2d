#define H2D_REPORT_WARN
#define H2D_REPORT_INFO
#define H2D_REPORT_VERBOSE
#define H2D_REPORT_FILE "application.log"
#include "hermes2d.h"
#include "solver_umfpack.h"

using namespace RefinementSelectors;

// This example shows how to run adaptive hp-FEM, h-FEM and p-FEM with
// basic control parameters. The underlying problem is a planar model
// of an electrostatic micromotor (MEMS). You may want to experiment with 
// various types of adaptivity via the options H2D_P_ISO, H2D_P_ANISO, 
// H2D_H_ISO, H2D_H_ANISO, H2D_HP_ISO, H2D_HP_ANISO_H, H2D_HP_ANISO_P, 
// and H2D_HP_ANISO. See the User Documentation for more details. 
//   Uniform initial polynomial degree of mesh elements can be set using
// the variable P_INIT. Before using adaptivity, you have to define a refinement 
// selector as shown below. The function adapt() takes the selector
// as a parameter, along with THRESHOLD, STRATEGY, and MESH_REGULARITY. 
//   Additional control parameters are available, these will be demonstrated
// in the following tutorial examples. In this example, two types of convergence  
// graphs are created -- error estimate wrt. the number of degrees of freedom 
// (DOF), and error estimate wrt. CPU time. Later we will show how to output 
// the error wrt. exact solution when exact solution is available. 
//   This example also demonstrates how to define different material parameters
// in various parts of the computational domain, and how to measure time. 
//
// PDE: -div[eps_r(x,y) grad phi] = 0
//      eps_r = EPS_1 in Omega_1 (surrounding air)
//      eps_r = EPS_2 in Omega_2 (moving part of the motor)
//
// BC: phi = 0 V on Gamma_1 (left edge and also the rest of the outer boundary
//     phi = VOLTAGE on Gamma_2 (boundary of stator)
//
// The following parameters can be changed:

const bool SOLVE_ON_COARSE_MESH = false; // If true, coarse mesh FE problem is solved in every adaptivity step.
                                         // If false, projection of the fine mesh solution on the coarse mesh is used. 
const int P_INIT = 2;                    // Initial polynomial degree of all mesh elements.
const double THRESHOLD = 0.2;            // This is a quantitative parameter of the adapt(...) function and
                                         // it has different meanings for various adaptive strategies (see below).
const int STRATEGY = 1;                  // Adaptive strategy:
                                         // STRATEGY = 0 ... refine elements until sqrt(THRESHOLD) times total
                                         //   error is processed. If more elements have similar errors, refine
                                         //   all to keep the mesh symmetric.
                                         // STRATEGY = 1 ... refine all elements whose error is larger
                                         //   than THRESHOLD times maximum element error.
                                         // STRATEGY = 2 ... refine all elements whose error is larger
                                         //   than THRESHOLD.
                                         // More adaptive strategies can be created in adapt_ortho_h1.cpp.
const CandList CAND_LIST = H2D_HP_ANISO; // Predefined list of element refinement candidates. Possible values are
                                         // H2D_P_ISO, H2D_P_ANISO, H2D_H_ISO, H2D_H_ANISO, H2D_HP_ISO, H2D_HP_ANISO_H
                                         // H2D_HP_ANISO_P, H2D_HP_ANISO. See User Documentation for details.
const int MESH_REGULARITY = -1;   // Maximum allowed level of hanging nodes:
                                  // MESH_REGULARITY = -1 ... arbitrary level hangning nodes (default),
                                  // MESH_REGULARITY = 1 ... at most one-level hanging nodes,
                                  // MESH_REGULARITY = 2 ... at most two-level hanging nodes, etc.
                                  // Note that regular meshes are not supported, this is due to
                                  // their notoriously bad performance.
const double ERR_STOP = 0.1;      // Stopping criterion for adaptivity (rel. error tolerance between the
const double CONV_EXP = 1.0;      // Default value is 1.0. This parameter influences the selection of
                                  // cancidates in hp-adaptivity. See get_optimal_refinement() for details.
                                  // fine mesh and coarse mesh solution in percent).
const int NDOF_STOP = 60000;      // Adaptivity process stops when the number of degrees of freedom grows
                                  // over this limit. This is to prevent h-adaptivity to go on forever.

// problem constants
const int OMEGA_1 = 1;
const int OMEGA_2 = 2;
const int STATOR_BDY = 2;
const double EPS_1 = 1.0;         // Relative electric permittivity in Omega_1.
const double EPS_2 = 10.0;        // Relative electric permittivity in Omega_2.
const double VOLTAGE = 50.0;      // Voltage on the stator.

// boundary condition types
BCType bc_types(int marker)
{
  return BC_ESSENTIAL;
}

// Dirichlet boundary condition values
scalar essential_bc_values(int ess_bdy_marker, double x, double y)
{
  return (ess_bdy_marker == STATOR_BDY) ? VOLTAGE : 0.0;
}

template<typename Real, typename Scalar>
Scalar biform1(int n, double *wt, Func<Real> *u, Func<Real> *v, Geom<Real> *e, ExtData<Scalar> *ext)
{
  return EPS_1 * int_grad_u_grad_v<Real, Scalar>(n, wt, u, v);
}

template<typename Real, typename Scalar>
Scalar biform2(int n, double *wt, Func<Real> *u, Func<Real> *v, Geom<Real> *e, ExtData<Scalar> *ext)
{
  return EPS_2 * int_grad_u_grad_v<Real, Scalar>(n, wt, u, v);
}

int main(int argc, char* argv[])
{
  // Load the mesh.
  Mesh mesh;
  H2DReader mloader;
  mloader.load("motor.mesh", &mesh);

  // Initialize the shapeset and the cache.
  H1Shapeset shapeset;
  PrecalcShapeset pss(&shapeset);

  // Create an H1 space.
  H1Space space(&mesh, &shapeset);
  space.set_bc_types(bc_types);
  space.set_essential_bc_values(essential_bc_values);
  space.set_uniform_order(P_INIT);

  // Enumerate degrees of freedom.
  int ndof = assign_dofs(&space);

  // Initialize the weak formulation.
  WeakForm wf;
  wf.add_biform(callback(biform1), H2D_SYM, OMEGA_1);
  wf.add_biform(callback(biform2), H2D_SYM, OMEGA_2);

  // Visualize the solution, its gradient, and mesh.
  ScalarView sview("Scalar potential Phi", 0, 0, 400, 600);
  VectorView gview("Gradient of Phi", 410, 0, 400, 600);
  gview.set_min_max_range(0, 1e8);
  OrderView  oview("Mesh", 820, 0, 400, 600);

  // Matrix solver.
  UmfpackSolver solver;

  // DOF and CPU convergence graphs.
  SimpleGraph graph_dof, graph_cpu;

  // Initialize a refinement selector.
  H1ProjBasedSelector selector(CAND_LIST, CONV_EXP, H2DRS_DEFAULT_ORDER, &shapeset);

  // Adaptivity loop:
  int as = 1;
  bool done = false;
  TimePeriod cpu_time;
  Solution sln_coarse, sln_fine;
  do
  {
    info("---- Adaptivity step %d:", as); as++;

    // Time measurement.
    cpu_time.tick(H2D_SKIP);

    // Initialize the coarse mesh problem.
    LinSystem ls(&wf, &solver);
    ls.set_space(&space);
    ls.set_pss(&pss);

    // Initialize and solve the fine mesh problem.
    RefSystem rs(&ls);
    rs.assemble();
    rs.solve(&sln_fine);

    // Either solve on coarse mesh or project the fine mesh solution 
    // on the coarse mesh.
    if (SOLVE_ON_COARSE_MESH) {
      ls.assemble();
      ls.solve(&sln_coarse);
    }
    else ls.project_global(&sln_fine, &sln_coarse);

    // Time measurement.
    cpu_time.tick();

    // View the coarse mesh solution.
    sview.show(&sln_coarse);
    gview.show(&sln_coarse, &sln_coarse, H2D_EPS_NORMAL, H2D_FN_DX_0, H2D_FN_DY_0);
    oview.show(&space);

    // Time measurement.
    cpu_time.tick(H2D_SKIP);

    // Calculate element errors and total error estimate.
    H1Adapt hp(&space);
    hp.set_solutions(&sln_coarse, &sln_fine);
    double err_est = hp.calc_error() * 100;

    // Time measurement.
    cpu_time.tick();

    // Report results.
    info("ndof_coarse: %d, ndof_fine: %d, err_est: %g%%", space.get_num_dofs(), rs.get_space(0)->get_num_dofs(), err_est);

    // Add entry to DOF convergence graph.
    graph_dof.add_values(space.get_num_dofs(), err_est);
    graph_dof.save("conv_dof.dat");

    // Add entry to CPU convergence graph.
    graph_cpu.add_values(cpu_time.accumulated(), err_est);
    graph_cpu.save("conv_cpu.dat");

    // Time measurement.
    cpu_time.tick(H2D_SKIP);

    // If err_est too large, adapt the mesh.
    if (err_est < ERR_STOP) done = true;
    else {
      done = hp.adapt(&selector, THRESHOLD, STRATEGY, MESH_REGULARITY);
      ndof = assign_dofs(&space);
      if (ndof >= NDOF_STOP) done = true;
    }

    // Time measurement.
    cpu_time.tick();
  }
  while (done == false);
  verbose("Total running time: %g s", cpu_time.accumulated());

  // Show the fine mesh solution - the final result.
  sview.set_title("Fine mesh solution");
  sview.show(&sln_fine);
  gview.show(&sln_fine, &sln_fine, H2D_EPS_HIGH, H2D_FN_DX_0, H2D_FN_DY_0);

  // Wait for all views to be closed.
  View::wait();
  return 0;
}
