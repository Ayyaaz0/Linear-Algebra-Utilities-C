#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "la_matrix.h"
#include "la_solve.h"

// Makes a simple diagonally-dominant matrix 
static void generate_system(Matrix *A, Matrix *b, size_t n) {
    for (size_t i = 0; i < n; i++) {
        for (size_t j = 0; j < n; j++) {
            LA_AT(A, i, j) = (i == j) ? (double)n : 1.0;
        }
        LA_AT(b, i, 0) = 1.0;
    }
}

int main(void) {
    const size_t N = 1000;   

    Matrix A = (Matrix){0};
    Matrix b = (Matrix){0};
    Matrix x = (Matrix){0};

    if (la_matrix_init(&A, N, N) != LA_OK) {
        printf("Failed to allocate A\n");
        return 1;
    }
    if (la_matrix_init(&b, N, 1) != LA_OK) {
        printf("Failed to allocate b\n");
        return 1;
    }

    generate_system(&A, &b, N);

    clock_t start = clock();
    la_status st = la_solve(&x, &A, &b);
    clock_t end = clock();

    if (st != LA_OK) {
        printf("Solve failed\n");
        return 1;
    }

    double seconds = (double)(end - start) / CLOCKS_PER_SEC;

    printf("Solved %zux%zu system in %.3f seconds\n", N, N, seconds);

    la_matrix_free(&A);
    la_matrix_free(&b);
    la_matrix_free(&x);

    return 0;
}
