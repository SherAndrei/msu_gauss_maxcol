
void extract(double* lhs, double *rhs, const int v, const int h)
{
    int i, j, v2 = v & (~1), h2 = h & (~1);
    double c00, c01, c10, c11;

    for(i = 0; i < v2; i += 2) {
        for(j = 0; j < h2; j += 2) {
            c00 = rhs[i * h + j];
            c10 = rhs[(i + 1) * h + j];
            c01 = rhs[i * h + (j + 1)];
            c11 = rhs[(i + 1) * h + j + 1];

            lhs[i * h + j]           -= c00;
            lhs[(i + 1) * h + j]     -= c10;
            lhs[i * h + (j + 1)]     -= c01;
            lhs[(i + 1) * h + j + 1] -= c11;
        }
    }

    if(v2 < v) {
        for(j = 0; j < h2 + (h2 < h); j++) {
            c00 = rhs[v2 * h + j];
            lhs[v2 * h + j] -= c00;
        }
    }
    if(h2 < h) {
        for(i = 0; i < v2; i++) {
            c00 = rhs[i * h + h2];
            lhs[i * h + h2] -= c00;
        }
    }
}