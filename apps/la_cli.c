#include <stdio.h>
#include <stdlib.h>

#include "input.h"
#include "la_matrix.h"
#include "la_ops.h"

static void print_matrix(const Matrix *m) {
    for (size_t i = 0; i < m->rows; i++) {
        for (size_t j = 0; j < m->cols; j++) {
            printf("%8.2f ", LA_AT(m, i, j));
        }
        printf("\n");
    }
}

static la_status read_matrix(Matrix *m, const char *title) {
    printf("\n%s\n", title);

    int rows = get_int_in_range("Rows (1-10): ", 1, 10);
    int cols = get_int_in_range("Cols (1-10): ", 1, 10);

    la_status st = la_matrix_init(m, (size_t)rows, (size_t)cols);
    if (st != LA_OK) return st;

    for (size_t i = 0; i < m->rows; i++) {
        printf("Row %zu (%zu values):\n", i, m->cols);
        for (size_t j = 0; j < m->cols; j++) {
            char prompt[64];
            snprintf(prompt, sizeof(prompt), "  a[%zu][%zu] = ", i, j);
            LA_AT(m, i, j) = get_double(prompt);
        }
    }
    return LA_OK;
}

static void do_transpose(void) {
    Matrix A = {0}, T = {0};

    if (read_matrix(&A, "Transpose a matrix") != LA_OK) {
        printf("Failed to read matrix.\n");
        la_matrix_free(&A);
        return;
    }

    if (la_transpose(&T, &A) != LA_OK) {
        printf("Transpose failed.\n");
        la_matrix_free(&A);
        return;
    }

    printf("\nA:\n");
    print_matrix(&A);

    printf("\nA^T:\n");
    print_matrix(&T);

    la_matrix_free(&A);
    la_matrix_free(&T);
}

int main(void) {
    while (1) {
        printf("\n===== Linear Algebra Utilities (Demo CLI) =====\n");
        printf("1. Transpose\n");
        printf("0. Exit\n");
        printf("==============================================\n");

        int sel = get_int("Enter an option: ");
        switch (sel) {
            case 1: do_transpose(); break;
            case 0: return 0;
            default: printf("Unknown option.\n"); break;
        }
    }
}
