#pragma once

int solve(const int n, const int m, double* A, double* b, double* X);
double residual(const double* matrix, const double* r_part,const double* answer, const int n, const int m);
double difference( double* answer, const int dim);