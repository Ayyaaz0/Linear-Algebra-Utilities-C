#ifndef LA_MATRIX_H
#define LA_MATRIX_H

#include <stddef.h> // size_t

typedef struct {
    size_t rows;
    size_t cols;
    double *data;   // row-major contiguous: data[i*cols + j]
} Matrix;

typedef enum {
    LA_OK = 0,
    LA_ERR_DIM = 1,
    LA_ERR_ALLOC = 2,
    LA_ERR_SINGULAR = 3
} la_status;

#define LA_AT(m,i,j) ((m)->data[(i) * (m)->cols + (j)])

// Lifecycle
la_status la_matrix_init(Matrix *m, size_t rows, size_t cols);
void      la_matrix_free(Matrix *m);

// Utilities
la_status la_matrix_copy(Matrix *dst, const Matrix *src);
void      la_matrix_fill(Matrix *m, double value);

#endif
