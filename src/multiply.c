#include "multiply.h"

// a size: av * ah 
// b size: bv * bh 
int basic_multiply(const double* const a, const int av, const int ah, 
                   const double* const b, const int bv, const int bh,
                   double* const c)
{
	int i, j, k;
    double sum;
	if(bv != ah) {
		return -1;
	}
	for(i = 0; i < av; i++)	
		for(j = 0; j < bh; j++) {
			sum = 0.;
			for(k = 0; k < ah; k++)
				sum += a[i * ah + k] * b[k * bh + j];
            c[i * bh + j] = sum;
        }	
	return 0;
}

// a size: av * ah 
// b size: bv * bh 
// разница в том, что теперь мы во время цикла работаем больше, чем с одним элементом
// в данной функции получаем одновременно 4 c_{ij}
int conv_basic_multiply(const double* const a, const int av, const int ah, 
                        const double* const b, const int bv, const int bh,
                        double* const c)
{
	int r, t, q;
	//ближайшее число делящееся на 3, меньше, чем соответствующие
    const int av3 = (av / 3) * 3;
	const int bh3 = (bh / 3) * 3;
	// для вышеописанной логики
	double c00, c01, c02, c10, c11, c12, c20, c21, c22;
	if(ah != bv)
		return -1;
	//Зануляем c
	for(r = 0; r < av; r++)
		for(t = 0; t < bh; t++)
			c[r * bh + t] = 0.;

	for(r = 0; r < av3; r += 3)
		for(t = 0; t < bh3; t += 3) {
            c00 = 0., c01 = 0., c02 = 0.;
            c10 = 0., c11 = 0., c12 = 0.;
            c20 = 0., c21 = 0., c22 = 0.;
			for(q = 0; q < ah; q++) {
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
			c[(r + 0) * bh + (t + 0)] += c00;
			c[(r + 0) * bh + (t + 1)] += c01;
			c[(r + 0) * bh + (t + 2)] += c02;
			c[(r + 1) * bh + (t + 0)] += c10;
			c[(r + 1) * bh + (t + 1)] += c11;
			c[(r + 1) * bh + (t + 2)] += c12;
			c[(r + 2) * bh + (t + 0)] += c20;
			c[(r + 2) * bh + (t + 1)] += c21;
			c[(r + 2) * bh + (t + 2)] += c22;
		}
	// если не получилось разделить четно, то
	// повторяем процесс для последнего столбца и строчки
	// так, как делали раньше
	if(av3 < av) {
        for(r = av3; r < av; r++)
		    for(t = 0; t < bh; t++) {
			    c00 = 0.;
			    for(q = 0; q < ah; q++)
				    c00 += a[r * ah + q] * b[q * bh + t];
			    c[r * bh + t] = c00;
		}	
	}
	if(bh3 < bh) {
		for(r = 0; r < av; r++)	{
            for(t = bh3; t < bh; t++)
            {
            	c00 = 0.;
    			for(q = 0; q < ah; q++)
    				c00 += a[r * ah + q] * b[q * bh + t];
    			c[r * bh + t] = c00;
            }
        }
	}
    	return 0;
}


// m - размер блока
// a * b = c
int block_multiply(double* const a, 
                   double* const b, 
                   double* const c, 
                   const int n, const int m)
{
	//для циклов
	int i, j, s, r, t, q;
	//для размеров блоков матриц
	//размер A_{i,j} : ah * av 
	int av, ah, bh;
	//int bv всегда равен ah
	// количество блоков размера n
	const int k = n / m;
	// длина/высота остаточного блока
	const int l = n - k * m;
	// указатели на очередные блоки
	double *pa, *pb, *pc;
	// для суммирования
	double sum;
	
	// используем формулу 
	// C_{ij} = \sum^{k+1}_{s=1} A_{is}*B_{sj}
	for(i = 0; i * m < n; i++) {
		//считаем вертик. размер следующего блока A_{is}
		av = (i < k) ? m : l; 
		for(j = 0; j * m < n; j++) {
			//считаем гориз. размер следующего блока B_{sj}
			bh = (j < k) ? m : l;

			// создаем блок C_{ij} размера av * bh 
			pc = c + (i * n + j) * m;
			// зануляем его с помощью двойного цикла (т.о. он попадает в кэш) 
			for(r = 0; r < av; r++)
				for(t = 0; t < bh; t++)
					pc[r * n + t] = 0.; 
			
			// идем по формуле:
			for(s = 0; s * m < n; s++) {
				//считаем гориз. размер следующего блока A_{is}
				//считаем вертик. размер следующего блока B_{sj}
				/*bv = */ah = (s < k) ? m : l;
				// указатель на блок A_{is} размера av * ah
				pa = a + i * n * m + s * m;
				// указатель на блок B_{sj} размера bv * bh
				pb = b + s * n * m + j * m;

				// получаем очередной элемент матрицы C_{ij}
				// с помощью обыного умножения
				// C_{ij}^{(rt)} = \sum^{(ah = bv) -1}_{q=1} A_{is}^(r,q) * B_{sj}^{(q,t)}
				for(r = 0; r < av; r++)
					for(t = 0; t < bh; t++) {
						sum = 0; // ah == bv
						for(q = 0; q < ah; q++)
							sum += pa[r * n + q] * pb[q * n + t];
						pc[r * n + t] += sum;  
					}
			}
		}
	}
	return 0;
}

// m - размер блока
// разница в том, что теперь мы во время цикла работаем больше, чем с одним элементом
// в данной функции получаем одновременно 4 c_{ij}^(rt)
int conv_block_multiply(double* const a, 
                        double* const b, 
                        double* const c, 
                        const int n, const int m)
{
	//для циклов
	int i, j, s, r, t, q;
	//для размеров блоков матриц
	//размер A_{i,j} : ah * av 
	int av, ah, bh;
  //int bv всегда равен ah
	// блиэайшие четные меньшие чем соответствующие
	int av2, bh2;
	// количество блоков размера n
	const int k = n / m;
	// длина/высота остаточного блока
	const int l = n - k * m;
	// указатели на очередные блоки
	double *pa, *pb, *pc;
	// для суммирования
	double sum;
	// для вышеописанной логики
	double c00, c10, c01, c11; 

	// используем формулу 
	// C_{ij} = \sum^{k+1}_{s=1} A_{is}*B_{sj}
	for(i = 0; i * m < n; i++) {
		//считаем вертик. размер следующего блока A_{is}
		av = (i < k) ? m : l; 
		for(j = 0; j * m < n; j++) {
			//считаем гориз. размер следующего блока B_{sj}
			bh = (j < k) ? m : l;

			// создаем блок C_{ij} размера av * bh 
			pc = c + (i * n + j) * m;
			// зануляем его с помощью двойного цикла (т.о. он попадает в кэш) 
			for(r = 0; r < av; r++)
				for(t = 0; t < bh; t++)
					pc [r * n + t] = 0.; 
			
			// идем по формуле:
			for(s = 0; s * m < n; s++) {
				//считаем гориз. размер следующего блока A_{is}
				//считаем вертик. размер следующего блока B_{sj}
				/*bv = */ah = (s < k) ? m : l;
				// указатель на блок A_{is} размера av * ah
				pa = a + i * n * m + s * m;
				// указатель на блок B_{sj} размера bv * bh
				pb = b + s * n * m + j * m;

				// получаем очередной элемент матрицы C_{ij}
				// с помощью обыного умножения
				// C_{ij}^{(rt)} = \sum^{(ah = bv) -1}_{q=1} A_{is}^(r,q) * B_{sj}^{(q,t)}
				
				// ближайшее четное < av
				av2 = (av & (~1));
				bh2 = (bh & (~1));
				for(r = 0; r < av2; r += 2)
					for(t = 0; t < bh2; t += 2) {
					// pc^{r,t}, pc^{r,t+1}, pc^{r+1,t}, pc^{r+1,t+1}
						c00 = 0., c01 = 0., c10 = 0., c11 = 0.;
						for(q = 0; q < ah; q++) {
							c00 += pa[r * n + q]     * pb[q * n + t];
							c01 += pa[r * n + q]     * pb[q * n + (t + 1)];
							c10 += pa[(r+1) * n + q] * pb[q * n + t];
							c11 += pa[(r+1) * n + q] * pb[q * n + (t+ 1)];
						}
						pc[r * n + t]       	  += c00;
						pc[r * n + (t + 1)] 	  += c01;
						pc[(r + 1) * n + t] 	  += c10;
						pc[(r + 1) * n + (t + 1)] += c11;
					}
				// если не получилось разделить четно, то
				// повторяем процесс для последнего столбца и строчки
				// так, как делали раньше
				if(av2 < av) {
					sum = 0.;
					for(t = 0; t < bh; t++) {
						for (q = 0; q < ah; q ++)
							sum += pa[av * n + q] * pb[q * n + t];
						pc[av * n + t] += sum;
					}
				}
				if(bh2 < bh) {
					sum = 0.;
					for(r = 0; r < av; r++) {
						for(q = 0; q < ah; q++)
							sum += pa[r * n + q] * pb[q * n + bh];
						pc[r * n + bh] += sum;
					}
				}
			}
		}
	}
	return 0;
}