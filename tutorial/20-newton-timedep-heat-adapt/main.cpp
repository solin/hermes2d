#define H2D_REPORT_WARN
#define H2D_REPORT_INFO
#define H2D_REPORT_VERBOSE
#define H2D_REPORT_FILE "application.log"
#include "hermes2d.h"
#include "solver_umfpack.h"

using namespace RefinementSelectors;

//  This example shows how to combine automatic adaptivity with the Newton's
//  method for a nonlinear time-dependent PDE discretized implicitly in time
//  using implicit Euler or Crank-Nicolson.
//
//  PDE: time-dependent heat transfer equation with nonlinear thermal
//  conductivity, du/dt - div[lambda(u)grad u] = f
//
//  Domain: square (-10,10)^2
//
//  BC:  Dirichlet, given by the function dir_lift() below.
//  IC: Same function dir_lift().
//
//  The following parameters can be changed:

const bool NEWTON_ON_COARSE_MESH = false;  // true... Newton is done on coarse mesh in every adaptivity step
                                           // false...Newton is done on coarse mesh only once, then projection
                                           // of the fine mesh solution to coarse mesh is used
const int P_INIT = 2;                      // Initial polynomial degree of all mesh elements.
const int PROJ_TYPE = 1;                   // 1 for H1 projections, 0 for L2 projections
const int TIME_DISCR = 2;                  // 1 for implicit Euler, 2 for Crank-Nicolson
const double TAU = 0.5;                    // Time step
const double T_FINAL = 5.0;                // Time interval length
const int INIT_GLOB_REF_NUM = 2;           // Number of initial uniform mesh refinements

// Adaptivity
const int UNREF_FREQ = 1;                  // Every UNREF_FREQth time step the mesh is unrefined.
const double THRESHOLD = 0.3;              // This is a quantitative parameter of the adapt(...) function and
                                           // it has different meanings for various adaptive strategies (see below).
const int STRATEGY = 0;                    // Adaptive strategy:
                                           // STRATEGY = 0 ... refine elements until sqrt(THRESHOLD) times total
                                           //   error is processed. If more elements have similar errors, refine
                                           //   all to keep the mesh symmetric.
                                           // STRATEGY = 1 ... refine all elements whose error is larger
                                           //   than THRESHOLD times maximum element error.
                                           // STRATEGY = 2 ... refine all elements whose error is larger
                                           //   than THRESHOLD.
                                           // More adaptive strategies can be created in adapt_ortho_h1.cpp.
const CandList CAND_LIST = H2D_HP_ANISO;   // Predefined list of element refinement candidates. Possible values are
                                           // H2D_P_ISO, H2D_P_ANISO, H2D_H_ISO, H2D_H_ANISO, H2D_HP_ISO,
                                           // H2D_HP_ANISO_H, H2D_HP_ANISO_P, H2D_HP_ANISO.
                                           // See the User Documentation for details.
const int MESH_REGULARITY = -1;            // Maximum allowed level of hanging nodes:
                                           // MESH_REGULARITY = -1 ... arbitrary level hangning nodes (default),
                                           // MESH_REGULARITY = 1 ... at most one-level hanging nodes,
                                           // MESH_REGULARITY = 2 ... at most two-level hanging nodes, etc.
                                           // Note that regular meshes are not supported, this is due to
                                           // their notoriously bad performance.
const double CONV_EXP = 1.0;               // Default value is 1.0. This parameter influences the selection of
                                           // cancidates in hp-adaptivity. See get_optimal_refinement() for details.
const double ERR_STOP = 1.0;               // Stopping criterion for adaptivity (rel. error tolerance between the
                                           // fine mesh and coarse mesh solution in percent).
const int NDOF_STOP = 60000;               // Adaptivity process stops when the number of degrees of freedom grows
                                           // over this limit. This is to prevent h-adaptivity to go on forever.

// Newton's method
const double NEWTON_TOL_COARSE = 0.01;     // Stopping criterion for Newton on coarse mesh
const double NEWTON_TOL_FINE = 0.05;       // Stopping criterion for Newton on fine mesh
const int NEWTON_MAX_ITER = 20;            // Maximum allowed number of Newton iterations

// Thermal conductivity (temperature-dependent)
// Note: for any u, this function has to be positive
template<typename Real>
Real lam(Real u)
{
  return 1 + pow(u, 4);
}

// Derivative of the thermal conductivity with respect to 'u'
template<typename Real>
Real dlam_du(Real u) {
  return 4*pow(u, 3);
}

// This function is used to define Dirichlet boundary conditions
double dir_lift(double x, double y, double& dx, double& dy) {
  dx = (y+10)/10.;
  dy = (x+10)/10.;
  return (x+10)*(y+10)/100.;
}

// Initial condition
scalar initial_condition(double x, double y, double& dx, double& dy)
{
  return dir_lift(x, y, dx, dy);
}

// Boundary condition type
BCType bc_types(int marker)
{
  return BC_ESSENTIAL;
}

// Dirichlet boundary condition values
scalar essential_bc_values(int ess_bdy_marker, double x, double y)
{
  double dx, dy;
  return dir_lift(x, y, dx, dy);
}

// Heat sources (can be a general function of 'x' and 'y')
template<typename Real>
Real heat_src(Real x, Real y)
{
  return 1.0;
}

// Weak forms
# include "forms.cpp"

int main(int argc, char* argv[])
{
  // Load the mesh.
  Mesh mesh, basemesh;
  H2DReader mloader;
  mloader.load("square.mesh", &basemesh);

  // Initial mesh refinements.
  for(int i = 0; i < INIT_GLOB_REF_NUM; i++) basemesh.refine_all_elements();
  mesh.copy(&basemesh);

  // Initialize the shapeset and the cache.
  H1Shapeset shapeset;
  PrecalcShapeset pss(&shapeset);

  // Create finite element space.
  H1Space space(&mesh, &shapeset);
  space.set_bc_types(bc_types);
  space.set_essential_bc_values(essential_bc_values);
  space.set_uniform_order(P_INIT);

  // Enumerate degrees of freedom.
  int ndof = assign_dofs(&space);

  // Solutions for the time stepping and the Newton's method.
  Solution u_prev_time, u_prev_newton;

  // Initialize the weak formulation.
  WeakForm wf;
  if(TIME_DISCR == 1) {
    wf.add_biform(callback(J_euler), H2D_UNSYM, H2D_ANY, 1, &u_prev_newton);
    wf.add_liform(callback(F_euler), H2D_ANY, 2, &u_prev_newton, &u_prev_time);
  }
  else {
    wf.add_biform(callback(J_cranic), H2D_UNSYM, H2D_ANY, 1, &u_prev_newton);
    wf.add_liform(callback(F_cranic), H2D_ANY, 2, &u_prev_newton, &u_prev_time);
  }

  // Initialize the nonlinear system and solver.
  UmfpackSolver solver;
  NonlinSystem nls(&wf, &solver);
  nls.set_space(&space);
  nls.set_pss(&pss);

  // Visualize solution and mesh.
  ScalarView view("Initial condition", 0, 0, 410, 300);
  view.fix_scale_width(80);
  OrderView ordview("Initial mesh", 420, 0, 350, 300);

  // Error estimate and discrete problem size as a function of physical time.
  SimpleGraph graph_time_err, graph_time_dof;

  // Project the function initial_condition() on the coarse mesh.
  nls.project_global(initial_condition, &u_prev_time, PROJ_TYPE);
  u_prev_newton.copy(&u_prev_time);

  // View initial guess for Newton's method.
  view.show(&u_prev_newton);
  ordview.show(&space);

  // Newton's loop on the coarse mesh.
  info("---- Time step 1, Newton solve on coarse mesh:");
  if (!nls.solve_newton(&u_prev_newton, NEWTON_TOL_COARSE, NEWTON_MAX_ITER))
    error("Newton's method did not converge.");

  // Store the result in sln_coarse.
  Solution sln_coarse, sln_fine;
  sln_coarse.copy(&u_prev_newton);

  // Initialize refinement selector.
  H1ProjBasedSelector selector(CAND_LIST, CONV_EXP, H2DRS_DEFAULT_ORDER, &shapeset);

  // Time stepping loop.
  int nstep = (int)(T_FINAL/TAU + 0.5);
  for(int n = 1; n <= nstep; n++)
  {
    // Periodic global derefinements.
    if (n > 1 && n % UNREF_FREQ == 0) {
      info("---- Time step %d, global derefinement.", n);
      mesh.copy(&basemesh);
      space.set_uniform_order(P_INIT);
      ndof = assign_dofs(&space);

      // Project fine mesh solution on the globally derefined mesh.
      info("---- Time step %d, projecting fine mesh solution on globally derefined mesh:", n);
      nls.project_global(&sln_fine, &u_prev_newton, PROJ_TYPE);

      if (NEWTON_ON_COARSE_MESH) {
        // Newton's loop on the globally derefined mesh.
        info("---- Time step %d, Newton solve on globally derefined mesh:", n);
        if (!nls.solve_newton(&u_prev_newton, NEWTON_TOL_COARSE, NEWTON_MAX_ITER))
          error("Newton's method did not converge.");
      }

      // Store the result in sln_coarse.
      sln_coarse.copy(&u_prev_newton);
    }

    // Adaptivity loop:
    bool done = false;
    double err_est;
    int as = 1;
    do
    {
      info("---- Time step %d, adaptivity step %d, Newton solve on fine mesh:", n, as);

      // Reference nonlinear system.
      RefNonlinSystem rnls(&nls);
      rnls.prepare();

      // Set initial condition for the Newton's method on the fine mesh.
      if (as == 1) rnls.project_global(&sln_coarse, &u_prev_newton);
      else rnls.project_global(&sln_fine, &u_prev_newton);

      // Newton's method on fine mesh
      if (!rnls.solve_newton(&u_prev_newton, NEWTON_TOL_FINE, NEWTON_MAX_ITER))
        error("Newton's method did not converge.");

      // Store the result in sln_fine.
      sln_fine.copy(&u_prev_newton);

      // Visualize intermediate solution and mesh during adaptivity
      char title[100];
      sprintf(title, "Solution, time level %d, adapt step %d", n, as);
      view.set_title(title);
      view.show(&sln_coarse);
      sprintf(title, "Fine mesh, time level %d, adapt step %d", n, as);
      ordview.set_title(title);
      ordview.show(&space);

      // Calculate error estimate wrt. fine mesh solution.
      H1Adapt hp(&space);
      hp.set_solutions(&sln_coarse, &sln_fine);
      err_est = hp.calc_error() * 100;
      info("ndof_coarse: %d, ndof_fine: %d, err_est: %g%%", space.get_num_dofs(), rnls.get_space(0)->get_num_dofs(), err_est);

      // If err_est too large, adapt the mesh.
      if (err_est < ERR_STOP) done = true;
      else {
        hp.adapt(&selector, THRESHOLD, STRATEGY, MESH_REGULARITY);
        ndof = assign_dofs(&space);
        if (ndof >= NDOF_STOP) done = true;

        // Project the fine mesh solution on the new coarse mesh.
        info("---- Time step %d, adaptivity step %d, projecting fine mesh solution on new coarse mesh:",
          n, as);
        nls.project_global(&sln_fine, &u_prev_newton, PROJ_TYPE);

        if (NEWTON_ON_COARSE_MESH) {
          // Newton's loop on the coarse mesh.
          info("---- Time step %d, adaptivity step %d, Newton solve on new coarse mesh:", n, as);
          if (!nls.solve_newton(&u_prev_newton, NEWTON_TOL_COARSE, NEWTON_MAX_ITER))
            error("Newton's method did not converge.");
        }

        // Store the result in sln_coarse.
        sln_coarse.copy(&u_prev_newton);

        as++;
      }
    }
    while (!done);

    // Add entries to convergence graphs.
    graph_time_err.add_values(n*TAU, err_est);
    graph_time_err.save("time_error.dat");
    graph_time_dof.add_values(n*TAU, space.get_num_dofs());
    graph_time_dof.save("time_dof.dat");

    // Copy new time level solution into u_prev_time.
    u_prev_time.copy(&sln_fine);
  }

  // Wait for all views to be closed.
  View::wait();
  return 0;
}
