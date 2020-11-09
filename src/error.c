#include <stdio.h>
#include "error.h"

int error(const int error) {
    switch (error) {
    case 1: {
        printf("Usage: ./a.out n m r s (filename)\n"
                "\t n - matrix dim       \t(1 ...  )\n"
                "\t m - block dim        \t(1 ... n)\n"
                "\t r - printing value   \t(0 ... n)\n"
                "\t s - number of formula\t(0 ... 4)\n"
                "\t if s == 0 input file's name\n");
        break;
    }
    case 2: {
        printf("Wrong matrix dimension!\n"
                "Correct dimension value > 0\n");
        break;
    }
    case 3: {
        printf("Wrong block dimension!\n"
                "Correct block dimension (1 ... matrix_dim)\n");
        break;
    }
    case 4: {
        printf("Wrong print value!\n"
                "Correct print value (0 ... matrix_dim)\n");
        break;
    }
    case 5: {
        printf("Malloc error!\n");
        break;
    }
    case 6: {
        printf("Error opening file!\n");
        break;
    }
    case 7: {
        printf("Incorrect input or not enough data!\n");
        break;
    }
    case 10: {
        printf("Algotithm is inaplicable!\n");
        break;
    }
    default:
        return 0;
    }
    return error;
}
