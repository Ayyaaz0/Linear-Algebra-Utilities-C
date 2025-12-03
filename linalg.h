#ifndef LINALG_H
#define LINALG_H

#define MAX 10  // Max rows/cols for matrices
#define MAX_MATRICES 10 

typedef struct {
    int rows;
    int cols;
    double data[MAX][MAX];
} Matrix;


// Public functions called from main.c:
void linalg_add_n(void);
void linalg_sub_n(void);
void linalg_multiply_n(void);
void linalg_transpose(void);
void linalg_determinant(void);

#endif
