#include <stdio.h>
#include "error.h"

int error(const int error) {
    switch (error) {
    case INV_INPUT: {
        printf("Usage: ./a.out n p m r s (filename)\n"
                "\t n - matrix dim:       \t1, 2 ...  \n"
                "\t p - thread amount:    \t1, 2 ... \n"
                "\t m - block dim:        \t1, 2 ... n\n"
                "\t r - printing value:   \t0, 1 ... n\n"
                "\t s - number of formula:\t0, 1 ... 4\n"
                "\t if s == 0 input file's name\n");
        break;
    }
    case ALLOC_ERROR: {
        printf("Malloc error!\n");
        break;
    }
    case OPEN_FILE_ERR: {
        printf("Error opening file!\n");
        break;
    }
    case INV_FILE_DATA: {
        printf("Incorrect input or not enough data!\n");
        break;
    }
    case INAPL_ALG: {
        printf("Algotithm is inaplicable!\n");
        break;
    }
    default:
        return 0;
    }
    return error;
}
