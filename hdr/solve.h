#pragma once

int solve(const int dim, double* matrix, double* answer);
double residual(const int dim, const double* matrix,const double* r_part,const double* answer);
double difference(const int dim, double* answer);
double norm(const double* vector, const int length);