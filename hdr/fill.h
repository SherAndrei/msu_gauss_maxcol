#pragma once

void  fill(double* matrix, const int dim, const int formula_name, const char* const filename, int* errno);
void fill_right_part(const double* matrix, double* right_part, const int dim);