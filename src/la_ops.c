#include "la_ops.h"

la_status la_add(Matrix *out, const Matrix *a, const Matrix *b) {
    if (!out || !a || !b) return LA_ERR_DIM;
    if (a->rows != b->rows || a->cols != b->cols) return LA_ERR_DIM;

    la_status st = la_matrix_init(out, a->rows, a->cols);
    if (st != LA_OK) return st;

    for (size_t i = 0; i < a->rows; i++) {
        for (size_t j = 0; j < a->cols; j++) {
            LA_AT(out, i, j) = LA_AT(a, i, j) + LA_AT(b, i, j);
        }
    }
    return LA_OK;
}

la_status la_transpose(Matrix *out, const Matrix *a) {
    if (!out || !a) return LA_ERR_DIM;

    la_status st = la_matrix_init(out, a->cols, a->rows);
    if (st != LA_OK) return st;

    for (size_t i = 0; i < a->rows; i++) {
        for (size_t j = 0; j < a->cols; j++) {
            LA_AT(out, j, i) = LA_AT(a, i, j);
        }
    }
    return LA_OK;
}
