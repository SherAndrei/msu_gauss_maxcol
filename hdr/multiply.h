#pragma once

// a size: av * ah
// b size: bv * bh
int multiply(const double* const a, const int av, const int ah,
             const double* const b, const int bv, const int bh,
             double* const c);
