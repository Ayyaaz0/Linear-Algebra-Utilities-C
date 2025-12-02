#include "linalg.h"
#include "input.h"
#include <stdio.h>

static void input_matrix(Matrix *m);
static void print_matrix(const Matrix *m);
static void ask_matrix_dimensons(Matrix *m, const char *label);
static void read_matrix_fixed_size(Matrix *m, const char *title);
static void read_matrix_with_dims(Matrix *m, const char *title);   

static void zero_matrix(Matrix *m);
static void matrix_add_inplace(Matrix *dest, const Matrix *src);
static void matrix_sub_inplace(Matrix *dest, const Matrix *src);
static void multiply_matrices(const Matrix *a, const Matrix *b, Matrix *result);
static void transpose_matrix(const Matrix *m, Matrix *t);

void linalg_add_n(void) {
  int count =
      get_int_in_range("How many matrices do you want to add? (2-10) ", 2, MAX_MATRICES);
  Matrix sum, input;

  // Choose size for all matrices
  ask_matrix_dimensons(&sum, "Enter a size for all matrices to add:");
  zero_matrix(&sum); // initialise to zeroes first

  // Read matrices and accumulate
  for (int k = 1; k <= count; k++) {
    char title[64];
    if (k == 1) {
      snprintf(title, sizeof(title), "Matrix %d (starting matrix)", k);
    } else {
      snprintf(title, sizeof(title), "Matrix %d (added to sum)", k);
    }

    input.rows = sum.rows;
    input.cols = sum.cols;
    read_matrix_fixed_size(&input, title);

    matrix_add_inplace(&sum, &input);
  }

  printf("\nFinal result (sum of %d matrices):\n", count);
  print_matrix(&sum);
}

void linalg_sub_n(void) {
  int count =
      get_int_in_range("How many matrices do you want to subtract? (2-10) ", 2, MAX_MATRICES);

  Matrix sum, input;
  ask_matrix_dimensons(&sum, "Enter size for all matrices to subtract:");
  read_matrix_fixed_size(&sum, "Matrix 1 (starting matrix)");  // Read first matrix into sum

  // Subtract the remaining matrices from sum
  for (int k = 2; k <= count; k++) {
    char title[64];
    snprintf(title, sizeof(title), "Matrix %d (to subtract)", k);

    input.rows = sum.rows;
    input.cols = sum.cols;
    read_matrix_fixed_size(&input, title);

    matrix_sub_inplace(&sum, &input);
  }

  printf("\nFinal result (M1 minus the other %d matrices):\n", count - 1);
  print_matrix(&sum);
}

void linalg_multiply_n(void) {
  int count =
      get_int_in_range("How many matrices to multiply? (2-10): ", 2, MAX_MATRICES);

  Matrix result, current, temp;

  // First matrix: ask for dimensions and values
  read_matrix_with_dims(&result, "Matrix 1 (starting matrix)");

  for (int k = 2; k <= count; k++) {
    char title[64];
    snprintf(title, sizeof(title), "Matrix %d", k);
    read_matrix_with_dims(&current, title);

    // Check dimensions of matrices to be compatible with each other
    if (result.cols != current.rows) {
      printf("Cannot multiply at step %d because current result is %d x %d, "
             "next matrix is %d x %d. Columns of result must equal rows of next.\n",
             k, result.rows, result.cols, current.rows, current.cols);
      return;
    }

    multiply_matrices(&result, &current, &temp); // temp = result * current
    result = temp;                               // copy temp matrix back into result
  }

  printf("\nResult of multiplying %d matrices:\n", count);
  print_matrix(&result);
}

void linalg_transpose(void) {
    Matrix input, output;
    
    printf("\nTranpose a Matrix:\n");
    read_matrix_with_dims(&input, "Enter matrix to transpose:");

    printf("\nOriginal Matrix (%d x %d):\n", input.rows, input.cols);
    print_matrix(&input);
    
    transpose_matrix(&input, &output);

    printf("\nTransposed Matrix (%d x %d):\n", output.rows, output.cols);
    print_matrix(&output);
}

//--------------------- Helper Functions -----------------------//
/*
A general matrix element is defined as a_ij (reference guide)
        a11 a12 a13 a14 ... a1j
        a21 a22 a23 a24 ... a2j
        a31 a32 a33 a34 ... a3j
        .    .   .   .
        .    .   .   .
        ai1 ai2 ai3 ai4
*/
static void input_matrix(Matrix *m) {
  for (int i = 0; i < m->rows; i++) {
    printf("Row %d (%d values):\n", i, m->cols);

    for (int j = 0; j < m->cols; j++) {
      char prompt[64];

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

// Ask user for rows & cols for a matrix.
// e.g. label = "Enter size for matrices: "
static void ask_matrix_dimensons(Matrix *m, const char *label) {
  if (label && label[0] != '\0') {
    printf("%s\n", label);
  }
  m->rows = get_int_in_range("Rows (1-10): ", 1, MAX);
  m->cols = get_int_in_range("Cols (1-10): ", 1, MAX);
}

// Read a matrix when its size is already known.
static void read_matrix_fixed_size(Matrix *m, const char *title) {
  if (title && title[0] != '\0') {
    printf("\n%s\n", title);
  }
  input_matrix(m);
}

// Set all elements of m to zero. Rows/cols has to be already set prior.
static void zero_matrix(Matrix *m) {
  for (int i = 0; i < m->rows; i++) {
    for (int j = 0; j < m->cols; j++) {
      m->data[i][j] = 0.0;
    }
  }
}

// Read a matrix where the user chooses size & values
static void read_matrix_with_dims(Matrix *m, const char *title) {
  if (title && title[0] != '\0') {
    printf("\n%s\n", title);
  }
  ask_matrix_dimensons(m, NULL);
  input_matrix(m);
}

static void matrix_add_inplace(Matrix *dest, const Matrix *src) {
  for (int i = 0; i < dest->rows; i++) {
    for (int j = 0; j < dest->cols; j++) {
      dest->data[i][j] += src->data[i][j];
    }
  }
}

static void matrix_sub_inplace(Matrix *dest, const Matrix *src) {
  for (int i = 0; i < dest->rows; i++) {
    for (int j = 0; j < dest->cols; j++) {
      dest->data[i][j] -= src->data[i][j];
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
      for (int k = 0; k < a->cols; k++) { // Iterate row by column
        result->data[i][j] += a->data[i][k] * b->data[k][j]; // Compute the dot product
      }
    }
  }
}

static void transpose_matrix(const Matrix *m, Matrix *t) {
    // Dimensions are flipped.
    t->rows = m->cols;
    t->cols = m->rows;

    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            t->data[j][i] = m->data[i][j];
        }
    }
}
