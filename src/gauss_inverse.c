#include <math.h>
#include "gauss_inverse.h"

// Поменять местами элемент lhs и rhs
static void swap(double* const lhs, double* const rhs) {
    double temp = *lhs;
    *lhs = *rhs;
    *rhs = temp;
}

#define A(i, j) A[(i) * n + (j)]
#define E(i, j) A_inversed[(i) * n + (j)]

int gauss_inverse(double * A, double* A_inversed, const int n, double ERROR) {
    // итераторы
    int i, j;
    const int n2 = n & (~1);
    // максимальный элемент по столбцу
    double max = 0.;
    // строчка и столбец с максимальным элементом
    int m = 0;
    // коэффициент
    double c = 0.;
    // идем по столбцам
    for (j = 0; j < n; j++) {
        // назначаем элемент на диагонали главным
        // и соответстенную строчку
        max = fabs(A(j, j));
        m = j;

        // ищем максимальный элемент в столбце
        // и строчку с ним
        for (i = j; i < n; i++) {
            if (fabs(A(i, j)) > max) {
                m = i;
                max   = fabs(A(i, j));
            }
        }

        if (max <= ERROR) {
            return -1;
        }

        if (m != j) {
            // меняем строчки местами
            for (i = 0; i < j; i++) {
                swap(&(E(j, i)), &(E(m, i)));
            }
            for (i = j; i < n; i++) {
                swap(&(A(j, i)), &(A(m, i)));
                swap(&(E(j, i)), &(E(m, i)));
            }
        }
        // Так как элемент a_jj на диагонали != 0, то
        // разделим j уравнение на него
        c = 1. / A(j, j);
        if (fabs(c - 1) > ERROR) {
            for (m = 0; m < j + 1; m++)
                E(j, m) *= c;
            // A(j, j) = 1.;
            for (m = j + 1; m < n; m++) {
                A(j, m) *= c;
                E(j, m) *= c;
            }
        }

        // a_jj = 1
        // вычитаем из всех строчек строчку с максимальным элементом,
        // умноженную на соответствующий коэфф c, чтобы получить нули в столбце
        for (i = j + 1; i < n; i++) {
            c = A(i, j);
            if (fabs(c) > ERROR) {
                for (m = 0; m < j + 1; m++) {
                    E(i, m) -= c * E(j, m);
                }
                // A(i, j) = 0.;
                for (m = j + 1; m < n; m++) {
                    A(i, m) -= c * A(j, m);
                    E(i, m) -= c * E(j, m);
                }
            }
        }
    }
    // матрица теперь верхнедиагональная
    // обратный ход метода Гаусса
    // идем по столбцам
    for (j = n - 1; j >= 0; j--) {
        for (i = 0; i < j; i++) {
            c = A(i, j);
            if (fabs(c) > ERROR) {
                for (m = 0; m < n2; m+=2) {
                    E(i, m)     -= c * E(j, m);
                    E(i, m + 1) -= c * E(j, m + 1);
                }
                if (n2 < n) {
                    E(i, n2) -= c * E(j, n2);
                }
                // A(i, j) = 0.;
            }
        }
    }
    return 0;
}
