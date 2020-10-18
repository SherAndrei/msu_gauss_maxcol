#include "formula.h"
#include "matrix.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define MAX(a,b) (((a) > (b)) ? (a) : (b))

void formula_zero(double* A, const int n, const int m, 
				const char* const filename, int* errno)
{
	FILE* inp = fopen(filename, "r");
    if(inp == NULL) {
        *errno = 6;
		return;
	}

	int i, j, p, q, av, ah;
	// количество блоков размера n
	int k = n / m;
	// длина/высота остаточного блока
	int l = n - k * m;
	int counter = 0;

	for(i = 0; i * m < n; i++) {
		av = i < k ? m : l;
		for(p = 0; p < av; p++) {
			for(j = 0; j * m < n; j++) {
			ah = j < k ? m : l;
				for(q = 0; q < ah; q++) {
					if(fscanf(inp, "%lf", a(p, q)) == 1) 
						counter++;
					else break;
				}
			}
		}
	}
    if(counter != n * n)
        *errno = 7;
    fclose(inp);
}

//Формула 1 из условия
void formula_one(double* A, const int n, const int m)
{
	int i, j, p, q, av, ah;
	// количество блоков размера n
	int k = n / m;
	// длина/высота остаточного блока
	int l = n - k * m;

	for(i = 0; i * m < n; i++) {
		av = i < k ? m : l;
		for(j = 0; j * m < n; j++) {
			ah = j < k ? m : l;
			for(p = 0; p < av; p++) {
				for(q = 0; q < ah; q++) {
					*a(p, q) = n - MAX( i * m + p + 1, j * m + q + 1) + 1;
				}
			}
		}
	}
}
//Формула 2 из условия
void formula_two(double* A, const int n, const int m)
{
	int i, j, p, q, av, ah;
	// количество блоков размера n
	int k = n / m;
	// длина/высота остаточного блока
	int l = n - k * m;

	for(i = 0; i * m < n; i++) {
		av = i < k ? m : l;
		for(j = 0; j * m < n; j++) {
			ah = j < k ? m : l;
			for(p = 0; p < av; p++) {
				for(q = 0; q < ah; q++) {
					*a(p, q) = MAX( i * m + p + 1, j * m + q + 1);
				}
			}
		}
	}
}
//Формула 3 из условия
void formula_three(double* A, const int n, const int m)
{
	int i, j, p, q, av, ah;
	// количество блоков размера n
	int k = n / m;
	// длина/высота остаточного блока
	int l = n - k * m;

	for(i = 0; i * m < n; i++) {
		av = i < k ? m : l;
		for(j = 0; j * m < n; j++) {
			ah = j < k ? m : l;
			for(p = 0; p < av; p++) {
				for(q = 0; q < ah; q++) {
					*a(p, q) = abs( i * m + p - (j * m + q));
				}
			}
		}
	}
}
//Формула 4 из условия
void formula_four(double* A, const int n, const int m)
{
	int i, j, p, q, av, ah;
	// количество блоков размера n
	int k = n / m;
	// длина/высота остаточного блока
	int l = n - k * m;

	for(i = 0; i * m < n; i++) {
		av = i < k ? m : l;
		for(j = 0; j * m < n; j++) {
			ah = j < k ? m : l;
			for(p = 0; p < av; p++) {
				for(q = 0; q < ah; q++) {
					*a(p,q) = fabs( 1. / (i * m + p + j * m + q + 1));
				}
			}
		}
	}
}