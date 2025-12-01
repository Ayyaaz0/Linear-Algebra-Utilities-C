#include "linalg.h"
#include "input.h"
#include <stdio.h>

static void input_matrix(Matrix *m);
static void print_matrix(const Matrix *m);
static void matrix_elementwise_op(Matrix *sum, const Matrix *m, double factor);
static void multiply_matrices(const Matrix *a, const Matrix *b, Matrix *result);

void linalg_add_n(void) {
  int count =
      get_int_in_range("How many matrices do you want to add? (2-10) ", 2, 10);

  Matrix sum, M;
  sum.rows = get_int_in_range("Rows (1-10):  ", 1, MAX);
  sum.cols = get_int_in_range("Cols (1-10):  ", 1, MAX);

  // Set the sum matrix to all zeroes
  for (int i = 0; i < sum.rows; i++) {
    for (int j = 0; j < sum.cols; j++) {
      sum.data[i][j] = 0.0;
    }
  }

  // Read matrices and accumulate
  for (int k = 1; k <= count; k++) {
    if (k == 1) {
      printf("\n===== MATRIX %d (starting matrix) =====\n", k);
    } else {
      printf("\n===== MATRIX %d (added to sum) =====\n", k);
    }

    M.rows = sum.rows;
    M.cols = sum.cols;

    input_matrix(&M);
    matrix_elementwise_op(&sum, &M, +1.0); // addition
  }

  printf("\nFinal Result (sum of %d matrices)\n", count);
  print_matrix(&sum);
}

void linalg_sub_n(void) {
  int count = get_int_in_range(
      "How many matrices do you want to subtract? (2-10) ", 2, 10);

  Matrix sum, M;
  sum.rows = get_int_in_range("Rows (1-10):  ", 1, MAX);
  sum.cols = get_int_in_range("Cols (1-10):  ", 1, MAX);

  // Read the first matrix into sum
  printf("\n===== MATRIX 1 (starting matrix) =====\n");
  input_matrix(&sum);

  // Subtract the remaining matrices from sum
  for (int k = 2; k <= count; k++) {
    printf("\n===== MATRIX %d (to subtract) =====\n", k);
    M.rows = sum.rows;
    M.cols = sum.cols;

    input_matrix(&M);
    matrix_elementwise_op(&sum, &M, -1.0); // subtraction
  }

  printf("\nFinal result (M1 minus the other %d matrices):\n", count - 1);
  print_matrix(&sum);
}

void linalg_multiply_n(void) {
  int count =
      get_int_in_range("How many matrices to multiply? (2-10): ", 2, 10);

  Matrix result;
  Matrix current;
  Matrix temp;

  printf("\n===== MATRIX 1 (starting matrix) =====\n");
  result.rows = get_int_in_range("Rows (1-10): ", 1, MAX);
  result.cols = get_int_in_range("Cols (1-10): ", 1, MAX);
  input_matrix(&result);

  for (int k = 2; k <= count; k++) {
    printf("\n===== Matrx %d ====== \n", k);
    current.rows = get_int_in_range("Rows (1-10): ", 1, MAX);
    current.cols = get_int_in_range("Cols (1-10): ", 1, MAX);
    input_matrix(&current);

    // check matrices dimensions to be compatible with each other.
    if (result.cols != current.rows) {
      printf("Cannot multiply at step %d as current result is %d%d, "
             "next matrix is %d%d. (Columns of results must equal rows of next).\n",
             k, result.rows, result.cols, current.rows, current.cols);
      return;
    }
    multiply_matrices(&result, &current, &temp); // temp = result * current
    result = temp; // copy the temp matrices back into result.
  }
  printf("\nResults of multiplying %d matrices: \n", count);
  print_matrix(&result);
}

//--------------------- Helper Functions -----------------------//
/*
A general matrice's element is defined as a_ij (reference guide)
        a11 a12 a13 a14 ...a1j
        a21 a22 a23 a24 ...a2j
        a31 a32 a33 a34 ...a3j
        .    .   .   .
        .    .   .   .
        ai1 ai2 ai3 ai4
*/
static void input_matrix(Matrix *m) {
  for (int i = 0; i < m->rows; i++) {
    printf("Rows %d (%d values):\n", i, m->cols);

    for (int j = 0; j < m->cols; j++) {
      char prompt[PROMPT_LEN];

      snprintf(prompt, sizeof(prompt), "  a[%d][%d] = ", i, j);
      m->data[i][j] = get_double(prompt);
    }
  }
}

static void print_matrix(const Matrix *m) {
  for (int i = 0; i < m->rows; i++) {
    for (int j = 0; j < m->cols; j++) {
      printf("%8.2f ", m->data[i][j]);
    }
    printf("\n");
  }
}

static void matrix_elementwise_op(Matrix *sum, const Matrix *m, double factor) {
  for (int i = 0; i < sum->rows; i++) {
    for (int j = 0; j < sum->cols; j++) {
      sum->data[i][j] += factor * m->data[i][j];
    }
  }
}

static void multiply_matrices(const Matrix *a, const Matrix *b,
                              Matrix *result) {
  result->rows = a->rows;
  result->cols = b->cols;

  for (int i = 0; i < a->rows; i++) {
    for (int j = 0; j < b->cols; j++) {
      result->data[i][j] = 0.0;
      for (int k = 0; k < a->cols; k++) { //iterate row by column 
        result->data[i][j] += a->data[i][k] * b->data[k][j]; //compute the dot product
      }
    }
  }
}