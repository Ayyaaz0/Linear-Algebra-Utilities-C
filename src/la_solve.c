#include "la_solve.h"
#include <math.h>   // fabs

// Tolerance for treating a pivot as "effectively zero"
static const double LA_EPS = 1e-12;

static void swap_rows(Matrix *m, size_t r1, size_t r2) {
    if (r1 == r2) return;
    for (size_t j = 0; j < m->cols; j++) {
        double tmp = LA_AT(m, r1, j);
        LA_AT(m, r1, j) = LA_AT(m, r2, j);
        LA_AT(m, r2, j) = tmp;
    }
}

// Finds pivot row for column 'col' in rows [col..n-1] using partial pivoting.
// Returns 1 if pivot found, 0 if column is effectively zero.
static int find_pivot_row(const Matrix *A, size_t col, size_t *pivot_out) {
    const size_t n = A->rows;
    size_t pivot = col;
    double best = fabs(LA_AT(A, col, col));

    for (size_t r = col + 1; r < n; r++) {
        double v = fabs(LA_AT(A, r, col));
        if (v > best) {
            best = v;
            pivot = r;
        }
    }

    if (best < LA_EPS) return 0;
    *pivot_out = pivot;
    return 1;
}

la_status la_det(double *det_out, const Matrix *A) {
    if (!det_out || !A || !A->data) return LA_ERR_DIM;
    if (A->rows != A->cols) return LA_ERR_DIM;

    Matrix M = (Matrix){0};
    la_status st = la_matrix_copy(&M, A);
    if (st != LA_OK) return st;

    const size_t n = M.rows;
    int sign = 1;

    for (size_t col = 0; col < n; col++) {
        size_t pivot_row = col;

        if (!find_pivot_row(&M, col, &pivot_row)) {
            la_matrix_free(&M);
            *det_out = 0.0;
            return LA_ERR_SINGULAR;
        }

        if (pivot_row != col) {
            swap_rows(&M, pivot_row, col);
            sign = -sign;
        }

        const double pivot = LA_AT(&M, col, col);

        // Eliminate rows below pivot
        for (size_t r = col + 1; r < n; r++) {
            const double factor = LA_AT(&M, r, col) / pivot;
            LA_AT(&M, r, col) = 0.0; 

            for (size_t k = col + 1; k < n; k++) {
                LA_AT(&M, r, k) -= factor * LA_AT(&M, col, k);
            }
        }
    }

    double det = (double)sign;
    for (size_t i = 0; i < n; i++) {
        det *= LA_AT(&M, i, i);
    }

    la_matrix_free(&M);
    *det_out = det;
    return LA_OK;
}

la_status la_solve(Matrix *x_out, const Matrix *A, const Matrix *b) {
    if (!x_out || !A || !b || !A->data || !b->data) return LA_ERR_DIM; 
    if (x_out->data != NULL) return LA_ERR_DIM;

    la_matrix_reset(x_out); 

    if (A->rows != A->cols) return LA_ERR_DIM;   // A must be square
    if (b->cols != 1) return LA_ERR_DIM;         // b must be a column vector
    if (b->rows != A->rows) return LA_ERR_DIM;   // compatible sizes

    const size_t n = A->rows;

    // Build augmented matrix [A | b] of size n x (n+1)
    Matrix Aug = (Matrix){0};
    la_status st = la_matrix_init(&Aug, n, n + 1);
    if (st != LA_OK) return st;

    for (size_t i = 0; i < n; i++) {
        for (size_t j = 0; j < n; j++) {
            LA_AT(&Aug, i, j) = LA_AT(A, i, j);
        }
        LA_AT(&Aug, i, n) = LA_AT(b, i, 0);
    }

    // Forward elimination with partial pivoting
    for (size_t col = 0; col < n; col++) {
        size_t pivot_row = col;
        double best = fabs(LA_AT(&Aug, col, col));

        for (size_t r = col + 1; r < n; r++) {
            double v = fabs(LA_AT(&Aug, r, col));
            if (v > best) {
                best = v;
                pivot_row = r;
            }
        }

        if (best < LA_EPS) {
            la_matrix_free(&Aug);
            return LA_ERR_SINGULAR;
        }

        if (pivot_row != col) {
            swap_rows(&Aug, pivot_row, col);
        }

        const double pivot = LA_AT(&Aug, col, col);

        // Eliminate entries below pivot
        for (size_t r = col + 1; r < n; r++) {
            const double factor = LA_AT(&Aug, r, col) / pivot;
            LA_AT(&Aug, r, col) = 0.0;

            for (size_t k = col + 1; k < n + 1; k++) {
                LA_AT(&Aug, r, k) -= factor * LA_AT(&Aug, col, k);
            }
        }
    }

    // Allocate solution vector x (n x 1)
    st = la_matrix_init(x_out, n, 1);
    if (st != LA_OK) {
        la_matrix_free(&Aug);
        return st;
    }

    // Back substitution 
    for (size_t i = n; i-- > 0;) {
        const double rhs = LA_AT(&Aug, i, n);

        double sum = 0.0;
        for (size_t j = i + 1; j < n; j++) {
            sum += LA_AT(&Aug, i, j) * LA_AT(x_out, j, 0);
        }

        const double diag = LA_AT(&Aug, i, i);
        if (fabs(diag) < LA_EPS) {
            la_matrix_free(&Aug);
            la_matrix_free(x_out);
            return LA_ERR_SINGULAR;
        }

        LA_AT(x_out, i, 0) = (rhs - sum) / diag;
    }

    la_matrix_free(&Aug);
    return LA_OK;
}

la_status la_inverse(Matrix *A_inv, const Matrix *A) {
    if (!A_inv || !A || !A->data) return LA_ERR_DIM;
    if (A_inv->data != NULL) return LA_ERR_DIM;
    if (A->rows != A->cols) return LA_ERR_DIM;

    const size_t n = A->rows;

    // Allocate output inverse matrix
    la_status st = la_matrix_init(A_inv, n, n);
    if (st != LA_OK) return st;

    // Temporary vectors
    Matrix e = (Matrix){0};
    Matrix x = (Matrix){0};

    st = la_matrix_init(&e, n, 1);
    if (st != LA_OK) goto fail;

    // Solve A x = e_i for each column i
    for (size_t col = 0; col < n; col++) {
        // Reset e to zero
        for (size_t i = 0; i < n; i++) {
            LA_AT(&e, i, 0) = 0.0;
        }
        LA_AT(&e, col, 0) = 1.0;

        la_matrix_reset(&x);

        st = la_solve(&x, A, &e);
        if (st != LA_OK) goto fail;

        // Copy solution into column col of A_inv
        for (size_t i = 0; i < n; i++) {
            LA_AT(A_inv, i, col) = LA_AT(&x, i, 0);
        }

        la_matrix_free(&x);
    }

    la_matrix_free(&e);
    return LA_OK;

fail:
    la_matrix_free(&e);
    la_matrix_free(&x);
    la_matrix_free(A_inv);
    return st;
}
