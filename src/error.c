#include "error.h"
#include <stdio.h>

static int error_code = 0;

//Печатает ошибку во входных данных
int print_input_error(const char* error_name, const int error_number) {                 
    printf("Wrong input %s: %d\n", error_name, error_number);
    return --error_code;
}

//Печатает ошибку в функциях программы
int print_function_error(const char* error_func_name) {                 
    printf("Error during %s\n", error_func_name);
    return --error_code;
}
