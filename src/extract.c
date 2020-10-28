#include "extract.h"

void extract(double* lhs, double *rhs, const int v, const int h)
{
    int i, j;
    // int v2 = v & (~1), h2 = h & (~1);
    const int v3 = (v / 3) * 3;
    const int h3 = (h / 3) * 3;

    for(i = 0; i < v3; i += 3) {
        for(j = 0; j < h3; j += 3) {
            lhs[(i + 0) * h + (j + 0)] -= rhs[(i + 0) * h + (j + 0)];
            lhs[(i + 1) * h + (j + 0)] -= rhs[(i + 1) * h + (j + 0)];
            lhs[(i + 2) * h + (j + 0)] -= rhs[(i + 2) * h + (j + 0)];
            lhs[(i + 0) * h + (j + 1)] -= rhs[(i + 0) * h + (j + 1)];
            lhs[(i + 1) * h + (j + 1)] -= rhs[(i + 1) * h + (j + 1)];
            lhs[(i + 2) * h + (j + 1)] -= rhs[(i + 2) * h + (j + 1)];
            lhs[(i + 0) * h + (j + 2)] -= rhs[(i + 0) * h + (j + 2)];
            lhs[(i + 1) * h + (j + 2)] -= rhs[(i + 1) * h + (j + 2)];
            lhs[(i + 2) * h + (j + 2)] -= rhs[(i + 2) * h + (j + 2)];
        }
    }

    // if(v - v3 == 2) {
    //     for(i = v3; i < v; i += 2) {
    //         for(j = 0; j < (h & (~1)); j += 2) {
    //             lhs[(i + 0) * h + (j + 0)] -= rhs[(i + 0) * h + (j + 0)];
    //             lhs[(i + 1) * h + (j + 0)] -= rhs[(i + 1) * h + (j + 0)];
    //             lhs[(i + 0) * h + (j + 1)] -= rhs[(i + 0) * h + (j + 1)];
    //             lhs[(i + 1) * h + (j + 1)] -= rhs[(i + 1) * h + (j + 1)];
    //         }
    //     }
    //     if((h & (~1)) < h) {
    //         for(i = v3; i < v; i ++) {
    //             lhs[(i + 0) * h + (h & (~1))] -= rhs[(i + 0) * h + (h & (~1))];
    //         }
    //     }

    // }

    if(v3 < v) {
        for(i = v3; i < v; i++) {
            for(j = 0; j < h3 + (h - h3); j++) {
                lhs[i * h + j] -= rhs[i * h + j];
            }
        }
    }
    if(h3 < h) {
        for(i = 0; i < v3; i++) {
            for(j = h3; j < h; j++) {
                lhs[i * h + j] -= rhs[i * h + j];
            }
        }
    }
}