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
    int count = get_int_in_range("How many matrices do you want to add? (2-10)", 2, 10);
    int rows = get_int_in_range("Rows (1-10): ", 1, MAX);
    int cols = get_int_in_range("Cols (1-10): ", 1, MAX);

    double S[MAX][MAX]; //sum matrix
    double M[MAX][MAX]; //temp matrix to read into
    
    // Set the sum matrix to all zeroes
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            S[i][j] = 0.0;
        }
    }
    // Read matrices and accumlate
    for (int k = 1; k <= count; k++) {
        printf("\n===== MATRX %d =====\n", k);
        input_matrix(M, rows, cols);

        // S = S + M
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                S[i][j] += M[i][j];
            }
        }
    }
    printf("\nResult of adding %d matrices:\n", count);
    print_matrix(S, rows, cols);
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

