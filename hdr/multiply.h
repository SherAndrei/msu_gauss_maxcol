#pragma once

// a size: av * ah 
// b size: bv * bh 
int basic_multiply(const double* const a, const int av, const int ah, 
                   const double* const b, const int bv, const int bh,
                   double* const c);
// a size: av * ah 
// b size: bv * bh 
// разница в том, что теперь мы во время цикла работаем больше, чем с одним элементом
// в данной функции получаем одновременно 4 c_{ij}
int conv_basic_multiply(const double* const a, const int av, const int ah, 
                        const double* const b, const int bv, const int bh,
                        double* const c);
// m - размер блока
// a * b = c
int block_multiply(double* const a, 
                   double* const b, 
                   double* const c, 
                   const int n, const int m);
// m - размер блока
// разница в том, что теперь мы во время цикла работаем больше, чем с одним элементом
// в данной функции получаем одновременно 4 c_{ij}^(rt)
int conv_block_multiply(double* const a, 
                        double* const b, 
                        double* const c, 
                        const int n, const int m);