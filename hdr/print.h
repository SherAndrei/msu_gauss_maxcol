#pragma once

void print_matrix(const double* A, const int ver, const int hor, const int m, const int r);
void print_all(double* matrix, const double* answer,  
               const int dim, const int r, float time);
void print_residual(const double* matrix, const double* r_part, const int dim,const double* answer);
void print_difference(const int dim, double* answer);