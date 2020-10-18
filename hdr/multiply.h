#pragma once
int basic_multiply(double *a, int av, int ah, double* b, int bv, int bh, double* c);
int block_multiply(double *a, double* b, double* c, int n, int m);
int conv_basic_multiply(double *a, int av, int ah, double* b, int bv, int bh, double* c);
int conv_block_multiply(double *a, double* b, double* c, int n, int m);