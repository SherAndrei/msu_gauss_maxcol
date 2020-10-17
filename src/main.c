/*
	Метод Гаусса с выбором максимального элемента по столбцу
	решения системы линейных уравнений
*/
#include "matrix.h"
#include "error.h"
#include "print.h"
#include "fill.h"
#include "solve.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

int main(int argc, const char* argv[])
{
	int errno = 0;
	int n, m, r, s;
	// Solving AX = B
	double *A = NULL, *B = NULL, *X = NULL;

	printf(" Usage: ");
	for(int i = 1; i < argc; i++) 
		printf("[%s] ", argv[i]);
	printf("\n\n");

    if(!((argc == 5 || argc == 6) &&
		(sscanf(argv[1], "%d", &n) == 1) &&
		(sscanf(argv[2], "%d", &m) == 1) &&
		(sscanf(argv[3], "%d", &r) == 1) &&
		(sscanf(argv[4], "%d", &s) == 1)))
			error(1);

	if(n < 0)           error(2);
	if(m > n || m <= 0) error(3);
	if(r > n || r < 0)  error(4);

	// выделяем память под матрицу и под правый вектор
	A = (double*)malloc((n * n + n) * sizeof(double)); 
	if(A == NULL) error(5);

	if(s == 0 && argc == 6) {
		fill(A, n, m, 0, argv[5], &errno);
		if(errno > 0) {
			free(A);
			error(errno);   
		}
	} else if ((s > 0 || s < 5) && argc == 5) {
		fill(A, n, m, s, NULL, NULL);
	} else {
		free(A);
		error(1);
	}
	
	B = (double*)malloc(n * 1 * sizeof(double));
	if(B == NULL) {
		free(A);
		error(5);
	}
	fill_right_part(A, B, n);

	X = (double*)malloc(n * 1 * sizeof(double));
	if(X == NULL) {
		free(A);
		free(B);
		error(5);
	}

	//Печатаем то, что было до
	print_matrix(A, n, n, m, r);
	// print_matrix(B, 1, n, r);

	//Засекаем время и решаем
	// time_t start, end;
	// start = clock();
	// solve(n, m, A, B, X);
	// end = clock();
    
	//Печатаем результат
	// print_all(A, X, n,
				// r, ((float)(end - start))/ CLOCKS_PER_SEC                
				// );

	// if(s == 0) {
	// 	fill(A, n, m, 0, argv[5], &errno);
	// 	if(errno > 0) {
	// 		free(A);
    //     	free(X);
	// 		free(B);
	// 		error(errno);   
	// 	}
	// } else 
	// 	fill(A, n, m, s, NULL, NULL);

	// fill_B(A, B, n);

	// print_residual(A, B, n, X);
	// print_difference(n, X);

	free(A);
	free(B);
	free(X);
}