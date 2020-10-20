#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// столбцовая норма матрицы
double  norm(const double* A, const int n)
{
	int i, j;
	double max = 0, current;
	for(i = 0; i < n; i++) {
		current = 0.;
		for(j = 0; j < n; j++) {
			current += fabs(A[i * n + j]);
		}
		if(current > max)
			max = current;
	}
    return max;
}

//Считываем первый элемент с файла (размерность) и возвращаем его
int get_dim(FILE* input)
{
    int result = 0;
    
    if(fscanf(input, "%d", &result) && result > 0)
        return result;
    return -1; 
}
//Создаем и заполняем массив, возвращаем указатель на его первый элемент
double* create_array(FILE* input, int num_of_elems)
{
    double* array = (double*)malloc(num_of_elems * sizeof(double));
    for(int i = 0; i < num_of_elems; ++i)
        fscanf(input, "%lf", &(array[i]));
    return array;
}

void print_concat(double* A, double* B, const int dim)
{
	for(int i = 0; i < dim; ++i)
    {
        for(int j = 0; j < dim; ++j)
            printf(" %2.3e", A[i * dim + j]);
        
        printf(" |");
        for(int j = 0; j < dim; ++j)
            printf(" %2.3e", B[i * dim + j]);
        
        printf("\n");
    }
    printf("\n\n");
}

//Печатаем массив
void print_matrix(double* matrix, int dim)
{
    for(int i = 0; i < dim; ++i)
    {
        for(int j = 0; j < dim; ++j)
            printf("%.5e ", matrix[i * dim + j]);
        
        printf("\n");
    }
    printf("\n\n");
}

//Поменять местами элемент lhs и rhs
void swap(double* lhs, double* rhs) 
{
    double temp = *lhs;
    *lhs = *rhs;
    *rhs = temp;
}

#define eps 1e-16

#define A(i,j) A[(i) * n + (j)]
#define E(i,j) A_inversed[(i) * n + (j)]

#define LOG 0

int gauss_inverse(double * A, double* A_inversed, const int n)
{
	// итераторы
	int i, j, k; 
	// максимальный элемент по столбцу
	double max, current;
	// строчка и столбец с максимальным элементом
	int max_i;

	double ERROR = norm(A, n) * eps;
	// коэффициент 
	double c;

	//идем по столбцам
	for(j = 0; j < n; j++) {

		// назначаем элемент на диагонали главным
		// и соответстенную строчку
		max = fabs(A(j,j));
		max_i = j;

		// ищем максимальный элемент в столбце
		// и строчку с ним
		for(i = j; i < n; i++) {
			if(fabs(A(i,j)) > max) {
				max_i = i;
				max   = A(i,j);
			}
		}

		if(fabs(max) <= ERROR) {
            return -1;
		}

		if(max_i != j) {
			// меняем строчки местами
			for(i = 0; i < j; i++)
				swap(&(E(j,i)), &(E(max_i, i)));			
			for(i = j; i < n; i++) {
				swap(&(A(j,i)), &(A(max_i, i)));			
				swap(&(E(j,i)), &(E(max_i, i)));			
			}
#if LOG
			printf("Swap %d and %d:\n", max_i, j);
			print_concat(A,A_inversed, n);
#endif
		}
		//Так как элемент a_jj на диагонали != 0, то
        //разделим j уравнение на него  
        c = A(j,j);
		if(fabs(c - 1) > ERROR) {
			for(k = 0; k < j; k++) {
				E(j, k) /= c;
			} 
        	for(k = j; k < n; k++) {
	    	    A(j, k) /= c;
	    	    E(j, k) /= c;
        	}
			ERROR /= fabs(c);
#if LOG
			printf("Divide %d line by %lf:\n", j, c);
			print_concat(A, A_inversed, n);
#endif
		}

		// a_jj = 1
        //вычитаем из всех строчек строчку с максимальным элементом,
        // умноженную на соответствующий коэфф c, чтобы получить нули в столбце 
		for(i = j + 1; i < n; i++) {
			c = A(i, j);
			if(fabs(c) > ERROR) {
				for(k = 0; k < j; k++) {
					E(i, k) -= c * E(j,k);
				}
				for(k = j; k < n; k++) {
					A(i, k) -= c * A(j,k);
					E(i, k) -= c * E(j,k);
				}
				ERROR += fabs(c) * ERROR;
#if LOG
				printf("Multiply %d line by %lf and extract from %d line:\n", j, c, i);
				print_concat(A, A_inversed, n);
#endif
			}
		}
	}
	// матрица теперь верхнедиагональная
	printf("Reverse:\n");
	//обратный ход метода Гаусса
	//идем по столбцам
	for(j = n - 1; j >= 0; j--) {
		for(i = 0; i < j; i++) {
			c = A(i,j);
			if(fabs(c) > ERROR) {
				for(k = 0; k < n; k++)
					E(i,k) -= c * E(j,k);
				A(i,j) = 0; 
#if LOG
				printf("Multiply %d line by %lf and extract from %d line:\n", j, c, i);
				print_concat(A, A_inversed, n);
#endif
			}
		}
	}
	return 0;
}

void make_identity(double* matr, const int dim)
{
	int i, j;
	for(i = 0; i < dim; i++) 
		for(j = 0; j < dim; j++)
			matr[i*dim + j] = ( i == j );
}

int main()
{
    FILE* input;
    input = fopen("a.txt", "r");

    int dim = get_dim(input);
    
    double* matrix = create_array(input, dim * dim);
	double* invers = (double*)malloc(dim*dim*sizeof(double));
	make_identity(invers, dim);
	print_concat(matrix, invers, dim);
	
    if(gauss_inverse(matrix, invers, dim) == -1) {
        printf("Matrix is singular!\n");
	} else {
    	print_concat(matrix, invers, dim);
	}
    
    free(matrix);
    free(invers);
    fclose(input);
    return 0;
}

#undef LOG