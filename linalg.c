#include <stdio.h>
#include <math.h>

#include "linalg.h"
#include "input.h"

static void input_matrix(Matrix *m);
static void print_matrix(const Matrix *m);
static void print_labeled_matrix(const char *label, const Matrix *m);

static void ask_matrix_dimensions(Matrix *m, const char *label);
static void read_matrix_fixed_size(Matrix *m, const char *title);
static void read_matrix_with_dims(Matrix *m, const char *title);   

static void zero_matrix(Matrix *m);
static void matrix_add_inplace(Matrix *dest, const Matrix *src);
static void matrix_sub_inplace(Matrix *dest, const Matrix *src);
static void multiply_matrices(const Matrix *a, const Matrix *b, Matrix *result);
static void transpose_matrix(const Matrix *m, Matrix *t);

static double determinant_gaussian(const Matrix *A);
static void swap_rows(Matrix *m, int r1, int r2);

void linalg_add_n(void) {
  int count = get_int_in_range("How many matrices do you want to add? (2-10) ", 2, MAX_MATRICES);
  Matrix sum, input, prev_sum;

  // Choose size for all matrices
  ask_matrix_dimensions(&sum, "Enter a size for all matrices to add:");
  zero_matrix(&sum); // initialise to zeroes first

  // Read matrices and accumulate
  for (int k = 1; k <= count; k++) {
    char title[64];
    snprintf(title, sizeof(title), "Matrix %d", k);

    input.rows = sum.rows;
    input.cols = sum.cols;
    read_matrix_fixed_size(&input, title);

    prev_sum = sum; // save previous result for formatting

    matrix_add_inplace(&sum, &input);

    // Format the output for user
    printf("\nStep %d. After adding matrix %d\n", k, k);
    print_labeled_matrix("Previous sum:", &prev_sum);
    print_labeled_matrix("Matrix to add by:", &input);
    print_labeled_matrix("New Sum:", &sum);
  }

  printf("\nFinal result (sum of %d matrices):\n", count);
  print_matrix(&sum);
}

void linalg_sub_n(void) {
  int count = get_int_in_range("How many matrices do you want to subtract? (2-10) ", 2, MAX_MATRICES);

  Matrix result, input, prev_result;
  ask_matrix_dimensions(&result, "Enter size for all matrices to subtract:");
  read_matrix_fixed_size(&result, "Matrix 1 (starting matrix)");  // Read first matrix into result

  // Subtract the remaining matrices from result
  for (int k = 2; k <= count; k++) {
    char title[64];
    snprintf(title, sizeof(title), "Matrix %d (to subtract)", k);

    input.rows = result.rows;
    input.cols = result.cols;
    read_matrix_fixed_size(&input, title);

    prev_result = result; //save previous result for formatting

    matrix_sub_inplace(&result, &input);

    //Format the output for the user
    printf("\nStep %d. After subtracting matrix %d\n" , k - 1, k);
    print_labeled_matrix("Previous result:", &prev_result);
    print_labeled_matrix("Matrix to subtract by:", &input);
    print_labeled_matrix("New result:", &result);
  }

  printf("\nFinal result (M1 minus the other %d matrices):\n", count - 1);
  print_matrix(&result);
}

void linalg_multiply_n(void) {
  int count =
      get_int_in_range("How many matrices to multiply? (2-10): ", 2, MAX_MATRICES);

  Matrix result, current, temp, prev_result;

  // First matrix: ask for dimensions and values
  read_matrix_with_dims(&result, "Matrix 1 (starting matrix)");

  printf("\nIntial result (after Matrix 1):\n");
  print_labeled_matrix("Result", &result);

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
    
    prev_result = result;

    multiply_matrices(&result, &current, &temp); // temp = result * current

    //Formatting for the user
    printf("\nStep %d. After multiplying by matrix %d\n", k - 1, k);
    print_labeled_matrix("Previous result:", &prev_result);
    print_labeled_matrix("Matrix to multiply by:", &current);
    print_labeled_matrix("New result:", &temp);

    result = temp;                               // copy temp matrix back into result
  }

  printf("\nResult of multiplying %d matrices:\n", count);
  print_matrix(&result);
}

void linalg_transpose(void) {
    Matrix input, output;
    
    printf("\nTranpose a matrix:\n");
    read_matrix_with_dims(&input, "Enter matrix to transpose:");

    transpose_matrix(&input, &output);

    printf("\nOriginal Matrix:\n");
    print_labeled_matrix("A", &input);

    printf("\nTransposed Matrix (%d x %d):\n", output.rows, output.cols);
    print_labeled_matrix("A^T", &output);
}

void linalg_determinant(void) {
  Matrix A;

  printf("Find the determinant of a matrix:\n");
  read_matrix_with_dims(&A, "Enter a square matrix:");

  if (A.rows != A.cols) {
    printf("The determinant is only defined for square matrices.\n");
    return;
  }

  double det = determinant_gaussian(&A);

  // Epsilon is just for printing aproximately zero.
  const double EPSILON = 1e-9;

  printf("\nMatrix A:\n");
  print_labeled_matrix("A", &A);

  if (fabs(det) < EPSILON) {
    printf("det(A) = 0.0 (matrix is singular)\n");
  } else {
    printf("det(A) = %.6f\n", det);
  }
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

static void print_labeled_matrix(const char *label, const Matrix *m) {
  if (label && label[0] != '\0') {
    printf("%s = \n", label);
  }
  print_matrix(m);
  printf("\n");
}

// Ask user for rows & cols for a matrix.
// e.g. label = "Enter size for matrices: "
static void ask_matrix_dimensions(Matrix *m, const char *label) {
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
  ask_matrix_dimensions(m, NULL);
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

static void swap_rows(Matrix *m, int r1, int r2) {
  if (r1 == r2) {
    return;
  }
  for (int k = 0; k < m->cols; k++) {
    // This is necessary to avoid overwriting the value before it can be used.
    double temp = m->data[r1][k];
    m->data[r1][k] = m->data[r2][k];
    m->data[r2][k] = temp;
  }
}

static double determinant_gaussian(const Matrix *A){
  // Assume that matrix A is square. It is asserted for safety.
  int n = A->rows;
  const double EPSILON = 1e-9;

  // Create a matrix copy such that original isn't modified.
  Matrix M = *A;

  int sign = 1;

  // The main loop iterates through each column to establish the pivot and eliminate elements below it.
  for (int col = 0; col < n; col++) {
    // Find the pivot row in this column
    int pivot_row = col;
    int found_pivot = 0;

    for (int r = col; r < n; r++) {
      if (fabs(M.data[r][col]) > EPSILON) {
        pivot_row = r;
        found_pivot = 1;
        break;
      }
    }

    if (!found_pivot) {
      // We can assume the collumn is definately zero --> determinant is 0!
      // If a non-zero pivot cannot be found then the matrix is singular.
      return 0.0;
    }

    // Here we can swap pivot row into position if needed.
    if (pivot_row != col) {
      swap_rows(&M, pivot_row, col);
      sign = -sign; // each row swap flips sign of determinant
    }

    double pivot = M.data[col][col];

    // Eliminate rows below the pivot
    for (int row = col + 1; row < n; row ++) {
      double factor = M.data[row][col] / pivot;
      for (int k = col; k < n; k++) {
        M.data[row][k] -= factor * M.data[col][k];
      }
    }
  }
  // After the main loop finishes, M is fully in upper triangular form (echelon form).
  printf("\nUpper triangular form:\n");
  print_labeled_matrix("U", &M); //for debugging

  // The determinant is sign * product of diagonal
  double det = (double)sign;
  for (int i = 0; i < n; i++){
    det *= M.data[i][i];
  }

  return det;
}
