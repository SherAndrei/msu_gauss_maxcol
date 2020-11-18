#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>

double find_average(FILE* file) {
    int i = 0;
    double avg = 0;
    double x;
    for (i = 1; fscanf(file, "%lf", &x) == 1; i++) {
        avg += (x - avg) / i;
    }
    if (!feof(file)) {
        return -1;
    }
    return avg;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Usage: %s filename\n", argv[0]);
        return -1;
    }
    FILE * f = fopen(argv[1], "r");
    if (!f) {
        printf("Cannot open file %s\n", argv[1]);
        return -2;
    }
    printf("%f\n", find_average(f));
    return 0;
}
