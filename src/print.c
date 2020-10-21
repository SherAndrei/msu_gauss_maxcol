#include "print.h"
#include <stdio.h>

#define MIN(a,b) (((a) < (b)) ? (a) : (b))

// Печать матрицы соответственно условию
// A size : h x w (rows x cols)
// A_{i,j} size: av * ah
void print_matrix(const double* A, const int v, const int h, const int m, const int r)
{
	// количество печатаемых элементов в ширину
	int nv = MIN(v, r);
	// количество печатаемых элементов в высоту
	int nh = MIN(h, r);
	// размер очередного блока
	int mv, mh;
	// количество печатаемых данных в данном блоке
	int prn_val_h = r;
	int prn_val_v = r; 
	//для циклов
	int i, j, p, q;
	//количество в последнем блоке
	int av, ah;
	// количество блоков размера m в высоту
	int kh = h / m;
	// количество блоков размера m в ширину
	int kv = v / m;
	// длина/высота остаточного блока
	int lh = nh - kh * m;
	// длина/высота остаточного блока
	int lv = nv - kv * m;

	for(i = 0; i * m < nh; i++) {
		av = i < kh ? m : lh;
		mv = MIN(av, prn_val_v);
		for(p = 0; p < mv; p++) {
			prn_val_h = r;
			for(j = 0; j * m < nv; j++) {
				ah = j < kv ? m : lv;
				mh = MIN(ah, prn_val_h);
				for(q = 0; q < mh; q++) {
					printf(" %10.3e", *(A + (i) * h * m + (j) * av * m  + (p) * ah + (q)));
				}
				prn_val_h -= ah;
			}
			putchar('\n');
		}
		prn_val_v -= av;
	}
	putchar('\n');
}
