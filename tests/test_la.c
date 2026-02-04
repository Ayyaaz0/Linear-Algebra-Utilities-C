#include <stdio.h>
#include <math.h>

#include "la_matrix.h"
#include "la_ops.h"
#include "la_solve.h"

static int nearly_equal(double a, double b) {
    return fabs(a - b) < 1e-9;
}

int main(void) {
    Matrix A  = (Matrix){0};
    Matrix B  = (Matrix){0};
    Matrix C  = (Matrix){0};
    Matrix T  = (Matrix){0};
    Matrix M  = (Matrix){0};
    Matrix S  = (Matrix){0};

    if (la_matrix_init(&A, 2, 2) != LA_OK) return 1;
    if (la_matrix_init(&B, 2, 2) != LA_OK) return 1;

    // A = [1 2; 3 4]
    LA_AT(&A,0,0)=1; LA_AT(&A,0,1)=2;
    LA_AT(&A,1,0)=3; LA_AT(&A,1,1)=4;

    // B = [10 20; 30 40]
    LA_AT(&B,0,0)=10; LA_AT(&B,0,1)=20;
    LA_AT(&B,1,0)=30; LA_AT(&B,1,1)=40;

    // Addition test 
    if (la_add(&C, &A, &B) != LA_OK) return 2;
    if (!nearly_equal(LA_AT(&C,1,1), 44)) return 3;
    la_matrix_free(&C);

    // Transpose test 
    if (la_transpose(&T, &A) != LA_OK) return 4;
    if (!nearly_equal(LA_AT(&T,0,1), 3)) return 5; // A(1,0) -> T(0,1)
    la_matrix_free(&T);

    // Multiplication test 
    // A*B = [70 100; 150 220]
    if (la_mul(&M, &A, &B) != LA_OK) return 6;
    if (!nearly_equal(LA_AT(&M,0,0), 70))  return 7;
    if (!nearly_equal(LA_AT(&M,0,1), 100)) return 8;
    if (!nearly_equal(LA_AT(&M,1,0), 150)) return 9;
    if (!nearly_equal(LA_AT(&M,1,1), 220)) return 10;
    la_matrix_free(&M);

    // Subtraction test 
    // A - B = [-9 -18; -27 -36]
    if (la_sub(&S, &A, &B) != LA_OK) return 11;
    if (!nearly_equal(LA_AT(&S,0,0), -9))  return 12;
    if (!nearly_equal(LA_AT(&S,0,1), -18)) return 13;
    if (!nearly_equal(LA_AT(&S,1,0), -27)) return 14;
    if (!nearly_equal(LA_AT(&S,1,1), -36)) return 15;
    la_matrix_free(&S);

    // Determinant test 
    // det([1 2; 3 4]) = -2
    double det = 0.0;
    la_status dst = la_det(&det, &A);
    if (dst != LA_OK) return 20;
    if (!nearly_equal(det, -2.0)) return 21;

    // Solve test 
    // A2 = [[3,2],[1,2]], b2 = [[5],[5]]
    // solution x = [0, 2.5]^T
    Matrix A2 = (Matrix){0};
    Matrix b2 = (Matrix){0};
    Matrix x  = (Matrix){0};

    if (la_matrix_init(&A2, 2, 2) != LA_OK) return 22;
    if (la_matrix_init(&b2, 2, 1) != LA_OK) return 23;

    LA_AT(&A2,0,0)=3; LA_AT(&A2,0,1)=2;
    LA_AT(&A2,1,0)=1; LA_AT(&A2,1,1)=2;

    LA_AT(&b2,0,0)=5;
    LA_AT(&b2,1,0)=5;

    if (la_solve(&x, &A2, &b2) != LA_OK) return 24;
    if (!nearly_equal(LA_AT(&x,0,0), 0.0)) return 25;
    if (!nearly_equal(LA_AT(&x,1,0), 2.5)) return 26;

    // ---- Inverse test ----
    // A = [1 2; 3 4]
    // A^-1 = [-2  1; 1.5 -0.5]
    Matrix Ainv = (Matrix){0};

    if (la_inverse(&Ainv, &A) != LA_OK) return 30;
    if (!nearly_equal(LA_AT(&Ainv,0,0), -2.0))  return 31;
    if (!nearly_equal(LA_AT(&Ainv,0,1),  1.0))  return 32;
    if (!nearly_equal(LA_AT(&Ainv,1,0),  1.5))  return 33;
    if (!nearly_equal(LA_AT(&Ainv,1,1), -0.5))  return 34;
    la_matrix_free(&Ainv);

    
    la_matrix_free(&x);
    la_matrix_free(&A2);
    la_matrix_free(&b2);

    la_matrix_free(&A);
    la_matrix_free(&B);

    printf("test_la: PASS\n");
    return 0;
}

