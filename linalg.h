#ifndef LINALG_H
#define LINALG_H

#define MAX 10  // max rows/cols for matrices
#define PROMPT_LEN 64 

typedef struct {
    int rows;
    int cols;
    double data[MAX][MAX];
} Matrix;


// Public functions called from main.c:
void linalg_add_n(void);
void linalg_sub_n(void);

#endif