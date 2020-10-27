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
	//ближайшее число делящееся на 3, меньше, чем соответствующие
	const int av2 = (av & (~1));
	const int bh2 = (bh & (~1));
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
    if(av3 < av2 && av2 < av) {
        for(r = av3; r < av; r += 2) {
            for(t = 0; t < bh2; t += 2) {
                c00 = 0., c01 = 0.;
                c10 = 0., c11 = 0.;
                for(q = 0; q < ah; q++) {
                    c00 += a[(r + 0) * ah + q] * b[q * bh + (t + 0)];
                    c01 += a[(r + 0) * ah + q] * b[q * bh + (t + 1)];
                    c10 += a[(r + 1) * ah + q] * b[q * bh + (t + 0)];
                    c11 += a[(r + 1) * ah + q] * b[q * bh + (t + 1)];
                }
                c[(r + 0) * bh + (t + 0)] = c00;
                c[(r + 0) * bh + (t + 1)] = c01;
                c[(r + 1) * bh + (t + 0)] = c10;
                c[(r + 1) * bh + (t + 1)] = c11;
            }
        }
        c22 = 0.;
        for(q = 0; q < ah; q++)
            c22 += a[av2 * ah + q] * b[q * bh + bh2];
        c[av2 * bh + bh2] = c22;
    }
    if(bh3 < bh2 && bh2 < bh) {
        for(r = 0; r < av2; r += 2)
            for(t = bh3; t < bh; t += 2) {
                c00 = 0., c01 = 0.;
                c10 = 0., c11 = 0.;
                for(q = 0; q < ah; q++) {
                    c00 += a[(r + 0) * ah + q] * b[q * bh + (t + 0)];
                    c01 += a[(r + 0) * ah + q] * b[q * bh + (t + 1)];
                    c10 += a[(r + 1) * ah + q] * b[q * bh + (t + 0)];
                    c11 += a[(r + 1) * ah + q] * b[q * bh + (t + 1)];
                }
                c[(r + 0) * bh + (t + 0)] = c00;
                c[(r + 0) * bh + (t + 1)] = c01;
                c[(r + 1) * bh + (t + 0)] = c10;
                c[(r + 1) * bh + (t + 1)] = c11;
            }
        c22 = 0.;
        for(q = 0; q < ah; q++)
            c22 += a[av2 * ah + q] * b[q * bh + bh2];
        c[av2 * bh + bh2] = c22;
    }

	if((av3 < av && av2 == av) || (av == 1)) {
        for(r = av3; r < av; r++) {
            for(t = 0; t < bh; t++) {
                c00 = 0.;
                for(q = 0; q < ah; q++) 
                    c00 += a[r * ah + q] * b[q * bh + t];
                c[r * bh + t] = c00;
            }
        }
	}
	if((bh3 < bh && bh2 == bh) || (bh == 1)) {
		for(r = 0; r < av; r++)	
			for(t = bh3; t < bh; t++) {
				c00 = 0.;
				for(q = 0; q < ah; q++)
					c00 += a[r * ah + q] * b[q * bh + t];
				c[r * bh + t] = c00;
		}	
	}
	return 0;
}

