#include "extract.h"

void extract(double* lhs, double *rhs, const int v, const int h)
{
    int i, j, v2 = v & (~1), h2 = h & (~1);

    for(i = 0; i < v2; i += 2) {
        for(j = 0; j < h2; j += 2) {
            lhs[i * h + j]           -= rhs[i * h + j];
            lhs[(i + 1) * h + j]     -= rhs[(i + 1) * h + j];
            lhs[i * h + (j + 1)]     -= rhs[i * h + (j + 1)];
            lhs[(i + 1) * h + j + 1] -= rhs[(i + 1) * h + j + 1];
        }
    }

    if(v2 < v) {
        for(j = 0; j < h2 + (h2 < h); j++) {
            lhs[v2 * h + j] -= rhs[v2 * h + j];
        }
    }
    if(h2 < h) {
        for(i = 0; i < v2; i++) {
            lhs[i * h + h2] -= rhs[i * h + h2];
        }
    }
}