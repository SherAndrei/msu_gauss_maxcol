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
	int matr_dim, block_dim, print_value, formula;
	double *matrix = NULL, *right_part = NULL, *answer = NULL;

//	char* filename;
	printf("Usage: ");
	for(int i = 1; i < argc; i++) 
		printf("[%s] ", argv[i]);
	printf("\n\n");

    if(!((argc == 5 || argc == 6) &&
		(sscanf(argv[1], "%d", &matr_dim)    == 1) &&
		(sscanf(argv[2], "%d", &block_dim)   == 1) &&
		(sscanf(argv[3], "%d", &print_value) == 1) &&
		(sscanf(argv[4], "%d", &formula)	    == 1)))
			error(1);

	if(matr_dim < 0) 
		error(2);

	if(block_dim > matr_dim || block_dim <= 0)
		error(3);

	if(print_value > matr_dim || print_value < 0)
		error(4);

	// выделяем память под матрицу и под правый вектор
	matrix = (double*)malloc((matr_dim * matr_dim + matr_dim) * sizeof(double)); 
	if(matrix == NULL)
            error(5);

	if(formula == 0 && argc == 6) {
		fill(matrix, matr_dim, block_dim, 0, argv[5], &errno);
		if(errno > 0) {
			free(matrix);
			error(errno);   
		}
	} else if ((formula > 0 || formula < 5) && argc == 5) {
		fill(matrix, matr_dim, block_dim, formula, NULL, NULL);
	} else {
		free(matrix);
		error(1);
	}
	
	right_part = (double*)malloc(matr_dim * 1 * sizeof(double));
	if(right_part == NULL) {
		free(matrix);
		error(5);
	}
	fill_right_part(matrix, right_part, matr_dim);

	answer = (double*)malloc(matr_dim * 1 * sizeof(double));
	if(answer == NULL) {
		free(matrix);
		free(right_part);
		error(5);
	}

        //Печатаем то, что было до
	print_matrix(matrix, matr_dim, matr_dim , print_value);
	print_matrix(right_part, 1, matr_dim, print_value);

	//Засекаем время и решаем
	time_t start, end;
	start = clock();
	solve(matr_dim, block_dim, matrix, right_part, answer);
	end = clock();
    
	//Печатаем результат
	print_all(matrix, answer, matr_dim,
				print_value, ((float)(end - start))/ CLOCKS_PER_SEC                
				);

	if(formula == 0) {
		fill(matrix, matr_dim, block_dim, 0, argv[5], &errno);
		if(errno > 0) {
			free(matrix);
        	free(answer);
			free(matrix);
			error(errno);   
		}
	} else 
		fill(matrix, matr_dim, block_dim, formula, NULL, NULL);

	fill_right_part(matrix, right_part, matr_dim);

	print_residual(matrix, right_part, matr_dim, answer);
	print_difference(matr_dim, answer);

	free(matrix);
	free(right_part);
	free(answer);
}