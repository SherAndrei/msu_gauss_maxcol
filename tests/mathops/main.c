#include <stdio.h>
#include <stdlib.h>
// #include "mult.h"
#include "extract.h"
#include "mult_and_extr.h"

double* create_array(FILE* input, int num_of_elems) {
    int i;
    double* array = (double*)malloc(num_of_elems * sizeof(double));
    for (i = 0; i < num_of_elems; ++i)
        if (fscanf(input, "%lf", &(array[i])) != 1) {
            free(array);
            return NULL;
        }
    return array;
}

void print_matrix(double* a, int av, int ah) {
    int i, j;
    for (i = 0; i < av; i++) {
        for (j = 0; j < ah; j++) {
            printf(" %.2lf", a[i*ah + j]);
        }
        putchar('\n');
    }
    putchar('\n');
}

void fill_array(double* array, size_t size, double val) {
    for (int i = 0; i < size; i++) {
        array[i] = val;
    }
}

int main() {
    FILE* inp;
    int av, ah, bv, bh;
    inp = fopen("a.txt", "r");
    if (inp == NULL) {
        printf("Cannot open file!\n");
        return -1;
    }
    if (fscanf(inp, "%d%d", &av, &ah) != 2) {
        printf("Error dimensions A!\n");
        fclose(inp);
        return -1;
    }
    double* A = create_array(inp, av * ah);
    if (A == NULL) {
        printf("Bad data A!\n");
        fclose(inp);
        return -1;
    }
    fclose(inp);
    inp = fopen("b.txt", "r");

    if (fscanf(inp, "%d%d", &bv, &bh) != 2) {
        printf("Error dimensions B!\n");
        free(A);
        fclose(inp);
        return -1;
    }

    double* B = create_array(inp, bv * bh);
    if (B == NULL) {
        printf("Bad data B!\n");
        fclose(inp);
        return -1;
    }
    fclose(inp);

    double* C = (double*)malloc(av * bh * sizeof(double));
    fill_array(C, av * bh, 3.);
    printf("%d %d \n", av, ah);
    print_matrix(A, av, ah);
    printf("%d %d \n", bv, bh);
    print_matrix(B, bv, bh);

    // multiply(A, av, ah, B, bv, bh, C);
    extract(A, B, av, ah);
    // if (multiply_and_extract(A, av, ah, B, bv, bh, C) < 0) {
        // printf("wrong!\n");
        // free(A);
        // free(B);
        // free(C);
        // return -1;
    // }
    print_matrix(A, av, ah);
    // print_matrix(C, av, bh);
    free(A);
    free(B);
    free(C);
    return 0;
}
