// main.c
#include <stdio.h>
#include <stdbool.h>
#include "input.h"
#include "linalg.h"

int main(void) {
    int selection;
    bool running = true;

    while (running) {
        printf("\n===== Linear Algebra Calculator =====\n");
        printf("1. Add n matrices\n");
        printf("2. Subtract N matrices\n");
        printf("0. Exit\n");
        printf("=====================================\n");

        selection = get_int("Enter an option: ");

        switch (selection) {
            case 1:
                linalg_add_n();
                break;
            case 2:
                linalg_sub_n();
                break;
            case 0:
                printf("EXIT\n");
                running = false;
                break;
            default:
                printf("Unknown option.\n");
                break;
        }
    }
    return 0;
}
