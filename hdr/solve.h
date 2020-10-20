#pragma once

int solve(const int n, const int m, double* A, double* b, double* X);
double residual(const int dim, const double* matrix,const double* r_part,const double* answer);
double difference(const int dim, double* answer);
double norm(const double* const matrix, const int dim);