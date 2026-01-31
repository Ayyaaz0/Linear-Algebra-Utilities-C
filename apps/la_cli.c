#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "input.h"
#include "la_matrix.h"
#include "la_ops.h"
#include "la_solve.h"

static void print_matrix(const Matrix *m) {
    for (size_t i = 0; i < m->rows; i++) {
        for (size_t j = 0; j < m->cols; j++) {
            printf("%10.4f ", LA_AT(m, i, j));
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

// Read a column vector b of size n x 1
static la_status read_vector(Matrix *b, size_t n, const char *title) {
    printf("\n%s\n", title);

    la_status st = la_matrix_init(b, n, 1);
    if (st != LA_OK) return st;

    for (size_t i = 0; i < n; i++) {
        char prompt[64];
        snprintf(prompt, sizeof(prompt), "  b[%zu] = ", i);
        LA_AT(b, i, 0) = get_double(prompt);
    }
    return LA_OK;
}

static void do_transpose(void) {
    Matrix A = (Matrix){0};
    Matrix T = (Matrix){0};

    if (read_matrix(&A, "Transpose a matrix") != LA_OK) {
        printf("Failed to read matrix.\n");
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

static void do_determinant(void) {
    Matrix A = (Matrix){0};
    double det = 0.0;

    if (read_matrix(&A, "Determinant of a matrix") != LA_OK) {
        printf("Failed to read matrix.\n");
        return;
    }

    if (A.rows != A.cols) {
        printf("Matrix must be square to compute determinant.\n");
        la_matrix_free(&A);
        return;
    }

    la_status st = la_det(&det, &A);
    if (st == LA_ERR_SINGULAR) {
        printf("\ndet(A) = 0 (singular matrix)\n");
    } else if (st != LA_OK) {
        printf("\nDeterminant failed (error %d).\n", (int)st);
    } else {
        printf("\ndet(A) = %.6f\n", det);
    }

    la_matrix_free(&A);
}

static void do_solve(void) {
    Matrix A = (Matrix){0};
    Matrix b = (Matrix){0};
    Matrix x = (Matrix){0};

    printf("\nSolve Ax = b\n");

    if (read_matrix(&A, "Enter square matrix A") != LA_OK) {
        printf("Failed to read A.\n");
        return;
    }

    if (A.rows != A.cols) {
        printf("A must be square.\n");
        la_matrix_free(&A);
        return;
    }

    if (read_vector(&b, A.rows, "Enter vector b") != LA_OK) {
        printf("Failed to read b.\n");
        la_matrix_free(&A);
        return;
    }

    la_status st = la_solve(&x, &A, &b);
    if (st == LA_ERR_SINGULAR) {
        printf("System is singular (no unique solution).\n");
    } else if (st != LA_OK) {
        printf("Solve failed (error %d).\n", (int)st);
    } else {
        printf("\nSolution x:\n");
        print_matrix(&x);
    }

    la_matrix_free(&A);
    la_matrix_free(&b);
    la_matrix_free(&x);
}

static void do_add(void) {
    Matrix A = (Matrix){0};
    Matrix B = (Matrix){0};
    Matrix C = (Matrix){0};

    if (read_matrix(&A, "Matrix A") != LA_OK) {
        printf("Failed to read A.\n");
        return;
    }
    if (read_matrix(&B, "Matrix B") != LA_OK) {
        printf("Failed to read B.\n");
        la_matrix_free(&A);
        return;
    }

    if (A.rows != B.rows || A.cols != B.cols) {
        printf("Dimension mismatch: A is %zux%zu, B is %zux%zu\n",
               A.rows, A.cols, B.rows, B.cols);
        la_matrix_free(&A);
        la_matrix_free(&B);
        return;
    }

    la_status st = la_add(&C, &A, &B);
    if (st != LA_OK) {
        printf("Add failed (error %d).\n", (int)st);
    } else {
        printf("\nA + B:\n");
        print_matrix(&C);
    }

    la_matrix_free(&A);
    la_matrix_free(&B);
    la_matrix_free(&C);
}

static void do_sub(void) {
    Matrix A = (Matrix){0};
    Matrix B = (Matrix){0};
    Matrix C = (Matrix){0};

    if (read_matrix(&A, "Matrix A") != LA_OK) {
        printf("Failed to read A.\n");
        return;
    }
    if (read_matrix(&B, "Matrix B") != LA_OK) {
        printf("Failed to read B.\n");
        la_matrix_free(&A);
        return;
    }

    if (A.rows != B.rows || A.cols != B.cols) {
        printf("Dimension mismatch: A is %zux%zu, B is %zux%zu\n",
               A.rows, A.cols, B.rows, B.cols);
        la_matrix_free(&A);
        la_matrix_free(&B);
        return;
    }

    la_status st = la_sub(&C, &A, &B);
    if (st != LA_OK) {
        printf("Subtract failed (error %d).\n", (int)st);
    } else {
        printf("\nA - B:\n");
        print_matrix(&C);
    }

    la_matrix_free(&A);
    la_matrix_free(&B);
    la_matrix_free(&C);
}

static void do_mul(void) {
    Matrix A = (Matrix){0};
    Matrix B = (Matrix){0};
    Matrix C = (Matrix){0};

    if (read_matrix(&A, "Matrix A") != LA_OK) {
        printf("Failed to read A.\n");
        return;
    }
    if (read_matrix(&B, "Matrix B") != LA_OK) {
        printf("Failed to read B.\n");
        la_matrix_free(&A);
        return;
    }

    if (A.cols != B.rows) {
        printf("Dimension mismatch: A is %zux%zu, B is %zux%zu\n",
               A.rows, A.cols, B.rows, B.cols);
        printf("For A*B, A.cols must equal B.rows.\n");
        la_matrix_free(&A);
        la_matrix_free(&B);
        return;
    }

    la_status st = la_mul(&C, &A, &B);
    if (st != LA_OK) {
        printf("Multiply failed (error %d).\n", (int)st);
    } else {
        printf("\nA * B:\n");
        print_matrix(&C);
    }

    la_matrix_free(&A);
    la_matrix_free(&B);
    la_matrix_free(&C);
}

int main(void) {
  while (true) {
    printf("\n===== Linear Algebra Utilities (Demo CLI) =====\n");
    printf("1. Add (A + B)\n");
    printf("2. Subtract (A - B)\n");
    printf("3. Multiply (A * B)\n");
    printf("4. Transpose\n");
    printf("5. Determinant\n");
    printf("6. Solve Ax = b\n");
    printf("0. Exit\n");
    printf("==============================================\n");

    int sel = get_int("Enter an option: ");
    switch (sel) {
    case 1: do_add(); break;
    case 2: do_sub(); break;
    case 3: do_mul(); break;
    case 4: do_transpose(); break;
    case 5: do_determinant(); break;
    case 6: do_solve(); break;
    case 0: return 0;
    default: printf("Unknown option.\n"); break;
    }
  }
}
