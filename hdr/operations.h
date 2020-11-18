#pragma once

// lhs -= rhs
void extract(double* lhs, double *rhs, const int v, const int h);

// a size: av * ah
// b size: bv * bh
int multiply(const double* const a, const int av, const int ah,
             const double* const b, const int bv, const int bh,
             double* const c);

// extract_from -= lhs * rhs
int multiply_and_extract(const double* const lhs, const int av, const int ah,
                          const double* const rhs, const int bv, const int bh,
                          double* const extract_from);
