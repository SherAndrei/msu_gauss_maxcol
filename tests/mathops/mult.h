// a size: av * ah 
// b size: bv * bh 
int basic_multiply(double* a, int av, int ah, double* b, int bv, int bh, double * c)
{
	int i, j, k;
	double sum;
	if(bv != ah) {
		return -1;
	}
	for(i = 0; i < av; i++)	
		for(j = 0; j < bh; j++) {
			sum = 0;
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
int conv_basic_multiply(double* a, int av, int ah, double* b, int bv, int bh, double * c)
{
	int r, t, q;
	double sum;
	//ближайшее четное, меньше, чем соответствующие
	int av2 = (av & (~1));
	int bh2 = (bh & (~1));
	// для вышеописанной логики
	double c00, c10, c01, c11;
	if(ah != bv)
		return -1;
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
		for(r = av2; r < av; r++)	
			for(t = 0; t < bh; t++) {
				sum = 0;
				for(q = 0; q < ah; q++)
					sum += a[r * ah + q] * b[q * bh + t];
				c[r * bh + t] = sum;
			}	
	}
	if(bh2 < bh) {
		for(r = 0; r < av; r++)	
			for(t = bh2; t < bh; t++) {
				sum = 0;
				for(q = 0; q < ah; q++)
					sum += a[r * ah + q] * b[q * bh + t];
				c[r * bh + t] = sum;
		}	
	}
	return 0;
}

