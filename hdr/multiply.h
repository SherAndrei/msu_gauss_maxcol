#pragma once

int basic_multiply(const double* const a, const int av, const int ah, 
                   const double* const b, const int bv, const int bh,
                   double* const c);

int conv_basic_multiply(const double* const a, const int av, const int ah, 
                        const double* const b, const int bv, const int bh,
                        double* const c);

int block_multiply(double* const a, 
                   double* const b, 
                   double* const c, 
                   const int n, const int m);

int conv_block_multiply(double* const a, 
                        double* const b, 
                        double* const c, 
                        const int n, const int m);