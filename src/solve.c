#include "solve.h"
#include "matrix.h"
#include "error.h"
#include "gauss_inverse.h"
#include "operations.h"

#ifdef BENCH
#include <time.h>
#include <stdio.h>
#endif
double fabs(double);

#define eps (1e-15)

#ifndef NULL
#define NULL ((void *)0)
#endif

// Найти корни и записать в answer
// help - дополнительная память под три блока
int solve(const int n, const int m,
          double* A, double* B, double* X,
          double* V1, double* V2, double* V3) {
#ifdef BENCH
    double temp, t_gauss = 0., t_swap = 0., t_mult = 0., t_form = 0., t_reverse = 0.;
#endif
    // итераторы
    int i = 0, j = 0, r = 0, q = 0;
    // вспомогательные указатели
    double *pa = NULL, *pi = NULL, *pj = NULL;
    // размер текущего блока av * ah
    int av = 0, ah = 0;
    // количество блоков
    const int k = n / m;
    // остаток
    const int l = n - k * m;
    // погрешность
    double ERROR = (norm(A, n) * eps);
    // минимальная норма обратной матрицы
    double min = 0.;
    // строчка с минимальной матрицей
    int min_i = 0;
    // счетчик необратимых матриц в столбце
    int c = 0;
    // текущая норма
    double current = 0.;

    for (j = 0; j * m < n; j++) {
        // A_{j, j} square!
        av = ah = j < k ? m : l;
        pa = A + j * n * m + j * av * m;

        // A_{j, j} --> V1
        // V_min = (A_{j, j})^(-1)
        // min = ||V_min||
#ifdef BENCH
        temp = clock();
#endif
        // copying pa --> V1 && identity(V3)
        for (r = 0; r < av; r++) {
            for (q = 0; q < ah; q++) {
                V1[r * ah + q] = pa[r * ah + q];
                V3[r * ah + q] = (r == q);
            }
        }
        if (gauss_inverse(V1, V3, av, ERROR) == 0) {
            min   = norm(V3, av);
            min_i = j;
        } else {
            min = 0.;
            c++;
        }

        // i = j + 1, ..., k - 1
        // A_{i, j} --> V1, V2 = (A_{i, j})^(-1)
        // if ||V2|| < min V3 = V2, min = ||V2||
        for (i = j + 1; i * m + l < n; i++) {
            pi = A + i * n * m + j * av * m;

            // copying pi --> V1 && identity(V2)
            for (r = 0; r < av; r++) {
                for (q = 0; q < ah; q++) {
                    V1[r * ah + q] = pi[r * ah + q];
                    V2[r * ah + q] = (r == q);
                }
            }
            // copy(pi, V1, av, ah);
            // identity(V2, av);
            if (gauss_inverse(V1, V2, av, ERROR) == 0) {
                current = norm(V2, av);
                if (current - min > ERROR) {
                    pj = V2;
                    V2 = V3;
                    V3 = pj;
                    min   = current;
                    min_i = i;
                }
            } else {
                c++;
            }
        }
#ifdef BENCH
        temp = (clock() - temp) / CLOCKS_PER_SEC;
        t_gauss += temp;
#endif
        // if all in column noninvertable return
        if (c == k - j + (av == l)) {
            return -1;
        }
#ifdef BENCH
        temp = clock();
#endif
        // I_min_i <--> I_j
        if (min_i != j) {
            for (i = 0; i * m < n; i++) {
                q  = (i < k) ? m : l;
                pi = A + min_i * n * m + i * m * m;
                pj = A + j * n * m + i * m * m;

                // swap pi and pj
                for (r = 0; r < m; r++) {
                    for (c = 0; c < q; c++) {
                        current       = pi[r * q + c];
                        pi[r * q + c] = pj[r * q + c];
                        pj[r * q + c] = current;
                    }
                }
            }
            pi = B + min_i * m;
            pj = B + j * m;
            // swap pi and pj
            for (r = 0; r < m; r++) {
                current = pi[r];
                pi[r]   = pj[r];
                pj[r]   = current;
            }
        }
#ifdef BENCH
        temp = (clock() - temp) / CLOCKS_PER_SEC;
        t_swap += temp;

        temp = clock();
#endif
        // A_{j, j} = E, V_3 * (A_{j, j+1},...,A_{j,k+1},B_{j})
        // identity(pa, av);
        for (i = j + 1; i * m < n; i++) {
            r = (i < k) ? m : l;
            pi = A + j * n * m + i * av * m;
            copy(pi, V1, av, r);
            multiply(V3, av, ah, V1, av, r, V2);
            copy(V2, pi, av, r);
        }
        pi = B + j * m;
        copy(pi, V1, av, 1);
        multiply(V3, av, ah, V1, av, 1, V2);
        copy(V2, pi, av, 1);
#ifdef BENCH
        temp = (clock() - temp) / CLOCKS_PER_SEC;
        t_mult += temp;

        temp = clock();
#endif
        // A_{ i, c } = A_{ i, c } - A_{ i, j } x A_{ j, c }
        //      pa    =     pa     -     pi     x     pj
        // идем по строчкам вниз
        for (i = j + 1; i * m < n; i++) {
            q = (i < k) ? m : l;
            pi = A + i * n * m + j * q * m;
            copy(pi, V1, q, m);
            // каждую умножаем и вычитаем с подходящим коэффицентом
            for (c = j + 1; c * m < n; c++) {
                ah = (c < k) ? m : l;
                pa = A + i * n * m + c * q  * m;
                pj = A + j * n * m + c * av * m;

                copy(pj, V2, m, ah);
                multiply_and_extract(V1, q, m, V2, m, ah, pa);
            }
            pa = B + i * m;
            pj = B + j * m;
            copy(pj, V2, m, 1);
            multiply_and_extract(V1, q, m, V2, m, 1, pa);
        }
#ifdef BENCH
        temp = (clock() - temp) / CLOCKS_PER_SEC;
        t_form += temp;
#endif

        // min = 0.;
        // min_i = 0;
        c = 0;
    }
    // матрица теперь верхнедиагональная

#ifdef BENCH
        t_reverse = clock();
#endif
    // последние X_{l} нам уже известны
    copy(B + k * m, X + k * m, l, 1);
    // идем с последней строчки
    for (i = k - 1; i >= 0; i--) {
        copy(B + i * m, X + i * m, m, 1);
        // сумма наших матриц
        for (j = i + 1; j * m < n; j++) {
            ah = (j < k) ? m : l;
            // A_{i, j}
            // уже не работаем с матрицами l x m
            pi = A + i * m * n + j * m * m;
            // X_{j}
            pj = X + j * m;

            multiply_and_extract(pi, m, ah, pj, ah, 1, (X + i * m));
        }
    }
#ifdef BENCH
        t_reverse = (clock() - t_reverse) / CLOCKS_PER_SEC;

    printf("gauss %f\n"
           "swap  %f\n"
           "mult  %f\n"
           "form  %f\n"
           "rev   %f\n", t_gauss, t_swap, t_mult, t_form, t_reverse);
#endif


    return 0;
}

// Норма невязки
double residual(double* A, double* B, double* X, const int n, const int m) {
    int i, j, q, r;
    int av, ah;
    double *pa, *pi, *pj;
    // количество блоков
    const int k = n / m;
    // остаток
    const int l = n - k * m;
    double norm = 0.;
    double sum = 0.;

    // Норма B
    for (i = 0; i < n; i++) {
        norm += fabs(B[i]);
    }
    // идем по столбцам
    for (j = 0; j * m < n; j++) {
        ah = (j < k) ? m : l;
        pj = X + j * m;
        // по каждой строчке
        for (i = 0; i * m < n; i++) {
            pi = B + i * m;
            av = (i < k) ? m : l;
            pa = A + i * m * n + j * av * m;
            // считаем B_{i} = B_{i} - \sum A_{i, j} X_{j}
            for (q = 0; q < av; q++) {
                sum = 0.;
                for (r = 0; r < ah; r++) {
                    sum += pa[q * ah + r] * pj[r];
                }
                pi[q] -= sum;
            }
        }
    }
    sum = 0.;
    // Норма невязки
    if (norm > eps) {
        for (i = 0; i < n; i++) {
            sum += fabs(B[i]);
        }
        sum /= norm;
    }

    return sum;
}

// Векторная норма разности с ответом
double difference(const double* const answer, const int dim) {
    double result = 0;
    int i;
    for (i = 0; i < dim; i++) {
        result += fabs(answer[i] - !(i & 1));
    }

    return result;
}

#undef eps
