#pragma once

// c -= a * b
int multiply_and_extract(const double* const a, const int av, const int ah,
                         const double* const b, const int bv, const int bh,
                         double* const c) {
    int r, t, q;
    // ближайшее число делящееся на 3, меньше, чем соответствующие
    const int av3 = (av / 3) * 3;
    const int bh3 = (bh / 3) * 3;
    // для вышеописанной логики
    double c00, c01, c02, c10, c11, c12, c20, c21, c22;
    if (ah != bv)
        return -1;

    for (r = 0; r < av3; r += 3)
        for (t = 0; t < bh3; t += 3) {
            c00 = 0., c01 = 0., c02 = 0.;
            c10 = 0., c11 = 0., c12 = 0.;
            c20 = 0., c21 = 0., c22 = 0.;
            for (q = 0; q < ah; q++) {
                c00 += a[(r + 0) * ah + q] * b[q * bh + (t + 0)];
                c01 += a[(r + 0) * ah + q] * b[q * bh + (t + 1)];
                c02 += a[(r + 0) * ah + q] * b[q * bh + (t + 2)];
                c10 += a[(r + 1) * ah + q] * b[q * bh + (t + 0)];
                c11 += a[(r + 1) * ah + q] * b[q * bh + (t + 1)];
                c12 += a[(r + 1) * ah + q] * b[q * bh + (t + 2)];
                c20 += a[(r + 2) * ah + q] * b[q * bh + (t + 0)];
                c21 += a[(r + 2) * ah + q] * b[q * bh + (t + 1)];
                c22 += a[(r + 2) * ah + q] * b[q * bh + (t + 2)];
            }
            c[(r + 0) * bh + (t + 0)] -= c00;
            c[(r + 0) * bh + (t + 1)] -= c01;
            c[(r + 0) * bh + (t + 2)] -= c02;
            c[(r + 1) * bh + (t + 0)] -= c10;
            c[(r + 1) * bh + (t + 1)] -= c11;
            c[(r + 1) * bh + (t + 2)] -= c12;
            c[(r + 2) * bh + (t + 0)] -= c20;
            c[(r + 2) * bh + (t + 1)] -= c21;
            c[(r + 2) * bh + (t + 2)] -= c22;
        }
    // если не получилось разделить четно, то
    // повторяем процесс для последнего столбца и строчки
    // так, как делали раньше
    if ((av - av3) == 2) {
        for (r = av3; r < av; r += 2) {
            for (t = 0; t < (bh & (~1)); t += 2) {
                c00 = 0., c01 = 0.;
                c10 = 0., c11 = 0.;
                for (q = 0; q < ah; q++) {
                    c00 += a[(r + 0) * ah + q] * b[q * bh + (t + 0)];
                    c01 += a[(r + 0) * ah + q] * b[q * bh + (t + 1)];
                    c10 += a[(r + 1) * ah + q] * b[q * bh + (t + 0)];
                    c11 += a[(r + 1) * ah + q] * b[q * bh + (t + 1)];
                }
                c[(r + 0) * bh + (t + 0)] -= c00;
                c[(r + 0) * bh + (t + 1)] -= c01;
                c[(r + 1) * bh + (t + 0)] -= c10;
                c[(r + 1) * bh + (t + 1)] -= c11;
            }
        }
        if ((bh & (~1)) < bh) {
            for (r = av3; r < av; r++)
                for (t = (bh & (~1)); t < bh; t++) {
                    c00 = 0.;
                    for (q = 0; q < ah; q++) {
                        c00 += a[(r + 0) * ah + q] * b[q * bh + (t + 0)];
                    }
                    c[(r + 0) * bh + (t + 0)] -= c00;
                }
        }
    }

    if ((bh - bh3) == 2) {
        for (r = 0; r < (av & (~1)); r += 2) {
            for (t = bh3; t < bh; t += 2) {
                c00 = 0., c01 = 0.;
                c10 = 0., c11 = 0.;
                for (q = 0; q < ah; q++) {
                    c00 += a[(r + 0) * ah + q] * b[q * bh + (t + 0)];
                    c01 += a[(r + 0) * ah + q] * b[q * bh + (t + 1)];
                    c10 += a[(r + 1) * ah + q] * b[q * bh + (t + 0)];
                    c11 += a[(r + 1) * ah + q] * b[q * bh + (t + 1)];
                }
                c[(r + 0) * bh + (t + 0)] -= c00;
                c[(r + 0) * bh + (t + 1)] -= c01;
                c[(r + 1) * bh + (t + 0)] -= c10;
                c[(r + 1) * bh + (t + 1)] -= c11;
            }
        }
        if ((av & (~1)) < av) {
            for (r = (av & (~1)); r < av; r++)
                for (t = bh3; t < bh; t++) {
                    c00 = 0.;
                    for (q = 0; q < ah; q++) {
                        c00 += a[(r + 0) * ah + q] * b[q * bh + (t + 0)];
                    }
                    c[(r + 0) * bh + (t + 0)] -= c00;
                }
        }
    }

    if (av - av3 == 1) {
        for (r = av3; r < av; r++)
            for (t = 0; t < bh; t++) {
                c00 = 0.;
                for (q = 0; q < ah; q++)
                    c00 += a[r * ah + q] * b[q * bh + t];
                c[r * bh + t] -= c00;
        }
    }
    if (bh - bh3 == 1) {
        for (r = 0; r < av; r++)    {
            for (t = bh3; t < bh; t++) {
                c00 = 0.;
                for (q = 0; q < ah; q++)
                    c00 += a[r * ah + q] * b[q * bh + t];
                c[r * bh + t] -= c00;
            }
        }
    }
    return 0;
}
