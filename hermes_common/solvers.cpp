// Copyright (c) 2009 hp-FEM group at the University of Nevada, Reno (UNR).
// Distributed under the terms of the BSD license (see the LICENSE
// file for the exact terms).
// Email: hermes1d@googlegroups.com, home page: http://hpfem.org/

#include "matrix.h"
#include "solvers.h"

// Standard CG method starting from zero vector
// (because we solve for the increment)
// x... comes as right-hand side, leaves as solution
bool CommonSolverCG::solve(Matrix* A, double *x, double tol, int maxiter)
{
    int n_dof = A->get_size();
    double *r = new double[n_dof];
    double *p = new double[n_dof];
    double *help_vec = new double[n_dof];
    if (r == NULL || p == NULL || help_vec == NULL) {
        _error("a vector could not be allocated in solve_linear_system_iter().");
    }
    // r = b - A*x0  (where b is x and x0 = 0)
    for (int i=0; i < n_dof; i++) r[i] = x[i];
    // p = r
    for (int i=0; i < n_dof; i++) p[i] = r[i];

    // setting initial condition x = 0
    for (int i=0; i < n_dof; i++) x[i] = 0;

    // CG iteration
    int iter_current = 0;
    double tol_current;
    while (1)
    {
        mat_dot(A, p, help_vec, n_dof);
        double r_times_r = vec_dot(r, r, n_dof);
        double alpha = r_times_r / vec_dot(p, help_vec, n_dof);
        for (int i=0; i < n_dof; i++) {
            x[i] += alpha*p[i];
            r[i] -= alpha*help_vec[i];
        }
        double r_times_r_new = vec_dot(r, r, n_dof);
        iter_current++;
        tol_current = sqrt(r_times_r_new);
        if (tol_current < tol
            || iter_current >= maxiter) break;
        double beta = r_times_r_new/r_times_r;
        for (int i=0; i < n_dof; i++) p[i] = r[i] + beta*p[i];
    }
    int flag;
    if (tol_current <= tol) flag = 1;
    else flag = 0;
    if (r != NULL) delete [] r;
    if (p != NULL) delete [] p;
    if (help_vec != NULL) delete [] help_vec;

    printf("CG (regular) made %d iteration(s) (tol = %g)\n",
           iter_current, tol_current);

    return flag;
}


bool CommonSolverCG::solve(Matrix* A, cplx *x)
{
    _error("CommonSolverCG::solve(Matrix *mat, cplx *res) not implemented.");
}

// ***********************************************************************************************************************

bool CommonSolverDenseLU::solve(Matrix* A, double *x)
{
    if (DenseMatrix *dmat = dynamic_cast<DenseMatrix*>(A))
    {
        bool free_dmat = false;
        if (dmat == NULL)
        {
            dmat = new DenseMatrix(A);
            free_dmat = true;
        }
        int n = dmat->get_size();
        int *indx = new int[n];
        double **_mat = dmat->get_A();
        double d;
        ludcmp(_mat, n, indx, &d);
        lubksb(_mat, n, indx, x);

        if (free_dmat)
            delete dmat;
    }
    else
        _error("Matrix type not supported.");
}

bool CommonSolverDenseLU::solve(Matrix* A, cplx *x)
{
    _error("CommonSolverDenseLU::solve(Matrix *mat, cplx *res) not implemented.");
}
