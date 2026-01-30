#include "la_matrix.h"
#include <stdlib.h> 

la_status la_matrix_init(Matrix *m, size_t rows, size_t cols) {
    if (!m || rows == 0 || cols == 0) return LA_ERR_DIM;

    m->rows = rows;
    m->cols = cols;
    m->data = (double *)malloc(rows * cols * sizeof(double));
    if (!m->data) {
        m->rows = 0;
        m->cols = 0;
        return LA_ERR_ALLOC;
    }
    return LA_OK;
}

void la_matrix_free(Matrix *m) {
    if (!m) return;
    free(m->data);
    m->data = NULL;
    m->rows = 0;
    m->cols = 0;
}

la_status la_matrix_copy(Matrix *dst, const Matrix *src) {
    if (!dst || !src) return LA_ERR_DIM;
    if (src->rows == 0 || src->cols == 0 || !src->data) return LA_ERR_DIM;

    la_status st = la_matrix_init(dst, src->rows, src->cols);
    if (st != LA_OK) return st;

    size_t n = src->rows * src->cols;
    for (size_t k = 0; k < n; k++) {
        dst->data[k] = src->data[k];
    }
    return LA_OK;
}

void la_matrix_fill(Matrix *m, double value) {
    if (!m || !m->data) return;
    size_t n = m->rows * m->cols;
    for (size_t k = 0; k < n; k++) {
        m->data[k] = value;
    }
}
