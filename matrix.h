#ifndef MATRIX_H
#define MATRIX_H

#include <string>

// Unfortunately, no one can be told what the Matrix is.
// You have to see it for yourself.

struct Matrix {
    float** data;
    int m;
    int n;

    Matrix (int m, int n);
    Matrix operator* (Matrix other);
    operator std::string () const;
    float* operator[] (int i);
};

#endif
