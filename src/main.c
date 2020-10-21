/*
	Метод Гаусса с выбором максимального элемента по столбцу
	решения системы линейных уравнений
*/
#include "matrix.h"
#include "error.h"
#include "print.h"
#include "fill.h"
#include "solve.h"
#include <stdio.h>
#include <time.h>

int main(int argc, const char* argv[])
{
	int errno = 0;
	int n, m, r, s;
	// Solving AX = B
	double *A = NULL, *B = NULL, *X = NULL;
	time_t start, end, start_solving, end_solving;
	printf(" Usage: ");
	for(int i = 1; i < argc; i++) 
		printf("[%s] ", argv[i]);
	printf("\n"); 

    if(!((argc == 5 || argc == 6) &&
		(sscanf(argv[1], "%d", &n) == 1) &&
		(sscanf(argv[2], "%d", &m) == 1) &&
		(sscanf(argv[3], "%d", &r) == 1) &&
		(sscanf(argv[4], "%d", &s) == 1)))
			return error(1);

	if(n < 0)           return error(2);
	if(m > n || m <= 0) return error(3);
	if(r > n || r < 0)  return error(4);

	// выделяем память под матрицу и под правый вектор
	A = alloc_matrix(n, n + 1); 
	if(A == NULL) return error(5);

	if(s == 0 && argc == 6) {
		fill(A, n, m, 0, argv[5], &errno);
		if(errno > 0) {
			free_matrix(A);
			return error(errno);   
		}
	} else if ((s > 0 && s < 5) && argc == 5) {
		fill(A, n, m, s, NULL, NULL);
	} else {
		free_matrix(A);
		return error(1);
	}
	
	B = A + n * n;
	fill_right_part(A, B, n, m);

	X = alloc_matrix(n, 1);
	if(X == NULL) {
		free_matrix(A);
		return error(5);
	}

	//Печатаем то, что было до
	print_matrix(A, n, n, m, r);
	print_matrix(B, n, 1, m, r);
	//Засекаем время и решаем
	
	start_solving = clock();
	if(solve(n, m, A, B, X) == 0) {
		
		printf("Solved!\n");
		print_matrix(A, n, n, m, r);
		print_matrix(X, n, 1, m, r);
	} else {
        free_matrix(A);
        free_matrix(X);
        return error(10);
	}
	end_solving = clock();
	
	if(s == 0) {
		fill(A, n, m, 0, argv[5], &errno);
		if(errno > 0) {
			free_matrix(A);
        	free_matrix(X);
			error(errno);   
		}
	} else 
		fill(A, n, m, s, NULL, NULL);

	fill_right_part(A, B, n, m);

	start = clock();
    printf(" Residual: %10.3e\n", residual(A, B, X, n, m));
	end = clock();
    printf(" Difference: %10.3e\n\n", difference(X, n));
	
    printf(" Time solving : %6.3f sec\n", ((float)(end_solving - start_solving))/ CLOCKS_PER_SEC);
    printf(" Time computing residual: %6.3f sec\n", ((float)(end - start))/ CLOCKS_PER_SEC);
	printf("________________________________________\n");
	free_matrix(A);
	free_matrix(X);
	return 0;
}
#undef LOG