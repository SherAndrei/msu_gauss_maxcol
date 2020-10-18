#include <stdio.h>
#include <stdlib.h>
// a size: av * ah 
// b size: bv * bh 
void basic_multiply(double* a, int av, int ah, double* b, int bv, int bh, double * c)
{
	int i, j, k;
	if(bv != ah) {
		//!!! free(c)
		c = (void*) 0;
		return;
	}
	for(i = 0; i < av; i++)	
		for(j = 0; j < bh; j++) {
			c[i * bh + j] = 0;
			for(k = 0; k < ah; k++)
				c[i * bh + j] += a[i * ah + k] * b[k * bh + j];
		}	
}

// a size: av * ah 
// b size: bv * bh 
// разница в том, что теперь мы во время цикла работаем больше, чем с одним элементом
// в данной функции получаем одновременно 4 c_{ij}
void conv_basic_multiply(double* a, int av, int ah, double* b, int bv, int bh, double * c)
{
	int r, t, q;
	double sum;
	//ближайшее четное, меньше, чем соответствующие
	int av2 = (av & (-1));
	int bh2 = (bh & (-1));
	// для вышеописанной логики
	double c00, c10, c01, c11;

	//Зануляем c
	for(r = 0; r < av; r ++)
		for(t = 0; t < bh; t ++)
			c[r * bh + t ] = 0;

	for(r = 0; r < av2; r += 2)
		for(t = 0; t < bh2; t += 2) {
			c00 = 0., c01 = 0., c10 = 0., c11 = 0.;
			for(q = 0; q < ah; q++) {
				c00 += a[r * ah + q]     * b[q * bh + t];
				c01 += a[r * ah + q]     * b[q * bh + (t + 1)];
				c10 += a[(r+1) * ah + q] * b[q * bh + t];
				c11 += a[(r+1) * ah + q] * b[q * bh + (t+ 1)];
			}
			c[r * bh + t]       	  += c00;
			c[r * bh + (t + 1)] 	  += c01;
			c[(r + 1) * bh + t] 	  += c10;
			c[(r + 1) * bh + (t + 1)] += c11;
		}
	// если не получилось разделить четно, то
	// повторяем процесс для последнего столбца и строчки
	// так, как делали раньше
	if(av2 < av) {
		sum = 0;
		for(t = 0; t < bh; t++) {
			for (q = 0; q < ah; q ++)
				sum += a[av2 * ah + q] * b[q * bh + t];
			c[av2 * bh + t] += sum;
		}
	}
	if(bh2 < bh) {
		sum = 0;
		for(r = 0; r < av; r++) {
			for(q = 0; q < ah; q++)
				sum += a[r * ah + q] * b[q * bh + bh2];
			c[r * bh2 + bh] += sum;
		}
	}
}

double* create_array(FILE* input, int num_of_elems)
{
    double* array = (double*)malloc(num_of_elems * sizeof(double));
    for(int i = 0; i < num_of_elems; ++i)
        fscanf(input, "%lf", &(array[i]));
    return array;
}

void print_matrix(double* a, int av, int ah)
{
	for(int i = 0; i < av; i++) {
		for(int j = 0; j < ah; j++) {
			printf(" %.2lf", a[i*ah + j]);
		}
		putchar('\n');
	}
	putchar('\n');
}

int main()
{
    FILE* inp;
	int av, ah, bv, bh;
    inp = fopen("a.txt", "r");
	if(fscanf(inp, "%d%d", &av, &ah) != 2) {
		printf("Error dimensions A!\n");
		fclose(inp);
		return -1;
	} 
	printf("%d %d \n", av, ah);
    double* A = create_array(inp, av * ah);

	fclose(inp);
	inp = fopen("b.txt", "r");

	if(fscanf(inp, "%d%d", &bv, &bh) != 2) {
		printf("Error dimensions B!\n");
		free(A);
		fclose(inp);
		return -1;
	} 
    double* B = create_array(inp, bv * bh);
	fclose(inp);

	double* C = (double*)malloc(av * bh * sizeof(double));
    print_matrix(A, av, ah);
    print_matrix(B, bv, bh);

	basic_multiply(A, av, ah, B, bv, bh, C);
	printf("Basic:\n");
    print_matrix(C, av, bh);
	printf("Conv:\n");
	conv_basic_multiply(A, av, ah, B, bv, bh, C);
    print_matrix(C, av, bh);

    free(A);
    free(B);
    free(C);
    return 0;
}