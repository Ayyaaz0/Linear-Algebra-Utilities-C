#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>

#include "input.h"

static void read_line(char *buffer, int size) {
    if (fgets(buffer, size, stdin) == NULL) {
        buffer[0] = '\0';
        return;
    }
    buffer[strcspn(buffer, "\n")] = '\0'; // remove newline character
}

int get_int(const char *prompt) {
    char buffer[128];
    long value;
    char *endptr;

    while (true) {
        printf("%s", prompt);
        read_line(buffer, sizeof(buffer));
        
        errno = 0;
        value = strtol(buffer, &endptr, 10);
        if (errno != 0 || endptr == buffer || *endptr != '\0'){
            printf("Enter a valid integer.\n");
            continue;
        }
        return (int)value;
    }
}

int get_int_in_range(const char *prompt, int min , int max) {
    int value;
    while(true){
        value = get_int(prompt);
        if (value < min || value > max){
            printf("Value must be between %d and %d.\n", min, max);
        } else {
            return value;
        }
    }
}

double get_double(const char *prompt) {
    char buffer[128];
    double value;
    char *endptr;

    while (true) {
        printf("%s", prompt);
        read_line(buffer, sizeof(buffer));

        errno = 0;
        value = strtod(buffer, &endptr);
        if (errno != 0 || endptr == buffer || *endptr != '\0'){
            printf("Enter a valid number.\n");
            continue;
        }
        return value;
    }
}