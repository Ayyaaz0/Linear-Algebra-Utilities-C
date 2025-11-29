#include <stdio.h>
#include "input.h"
#include "linalg.h"

static void add_matrices_menu(void);

static void input_matrix(double a[][MAX], int rows, int cols);
static void print_matrix(double a[][MAX], int rows, int cols);
static void add_matrices(double a[][MAX], double b[][MAX], double result[][MAX], int rows, int cols);



void linalg_menu(void) {
    int selection;
    do {
        printf("\n ==== Linear Algebra Tool ====\n");
        printf("1. Add Two Matrices\n");
        printf("0. Return to Main Menu\n");
        printf("===========================\n");

        selection = get_int("Select an option: ");

        switch (selection) {
            case 1: 
                add_matrices_menu();
                break;
            case 0: 
                printf("Returning to Main Menu\n"); 
                break;
            default: 
                printf("Unknown Option.\n"); 
                break;
        }
    } while (selection != 0);
}

static void add_matrices_menu(void) {
    int rows = get_int_in_range("Rows (1-10): ", 1, MAX);
    int cols = get_int_in_range("Cols (1-10): ", 1, MAX);

    double A[MAX][MAX], B[MAX][MAX], RESULT[MAX][MAX];
    
    printf("Enter matrix A:\n");
    input_matrix(A, rows, cols);
    printf("Enter matrix B:\n");
    input_matrix(B, rows, cols);

    add_matrices(A, B, RESULT, rows, cols);
    
    printf("Result (A + B):\n");
    print_matrix(RESULT, rows, cols);  
}

//--- Helper Functions ----
/*
A general matrice's element is defined as a_ij (reference guide)
        a11 a12 a13 a14 ...a1j
        a21 a22 a23 a24 ...a2j
        a31 a32 a33 a34 ...a3j
        .    .   .   .
        .    .   .   .
        ai1 ai2 ai3 ai4
*/
static void input_matrix(double a[][MAX], int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        printf("Rows %d (%d values):\n", i, cols);
        for (int j = 0; j < cols; j++) {
            char prompt[PROMPT_LEN];
            snprintf(prompt, sizeof(prompt), "  a[%d][%d] = ", i, j);
            a[i][j] = get_double(prompt);
        }
    }
}

static void print_matrix(double a[][MAX], int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%8.2f ", a[i][j]);
        }
        printf("\n");
    }
}

static void add_matrices(double a[][MAX], double b[][MAX], double result[][MAX], int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            result[i][j] = a[i][j] + b[i][j];
        }
    }
}

