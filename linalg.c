#include <stdio.h>
#include "input.h"

void linalg_menu(void) {
    int selection;
    do {
        printf("\n ==== Linear Algebra Tool ====\n");
        printf("1. Add two matrices\n");
        printf("0. Return to Main Menu\n");
        printf("===========================\n");

        
        selection = get_int("Select an option: ");

        switch (selection) {
            case 1: printf("Add Matrices Placeholder\n"); break;
            case 0: printf("Returning to Main Menu\n"); break;
            default: printf("Unknown Option.\n"); break;
        }
    } while (selection != 0);
}


