#ifndef INPUT_H
#define INPUT_H

#define MATRIX_MIN_VALUE (-1.e6)
#define MATRIX_MAX_VALUE (1e6)

int get_int(const char *prompt);
int get_int_in_range(const char *prompt, int min , int max);
double get_double(const char *prompt);

#endif
