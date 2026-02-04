#ifndef LA_SOLVE_H
#define LA_SOLVE_H

#include "la_matrix.h"

// Determinant of a square matrix A.
// Returns LA_OK, LA_ERR_DIM, LA_ERR_ALLOC, or LA_ERR_SINGULAR.
la_status la_det(double *det_out, const Matrix *A);

// Solve Ax = b where:
//  - A is n x n
//  - b is n x 1
//  - x_out will be allocated as n x 1
//
// Returns LA_OK, LA_ERR_DIM, LA_ERR_ALLOC, or LA_ERR_SINGULAR.
la_status la_solve(Matrix *x_out, const Matrix *A, const Matrix *b);

la_status la_inverse(Matrix *A_inv, const Matrix *A);

#endif
