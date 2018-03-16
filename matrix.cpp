#include <stdexcept>
#include "matrix.h"


Matrix::Matrix (int m, int n) : m{m}, n{n} {
    data = new float* [n];
    for (int i = 0; i < n; i++) {
       data[i] = new float [m] {};
    }
}

Matrix Matrix::operator* (Matrix other) {
    if (n != other.m) {
        throw new std::domain_error("Tamanhos inválidos de matriz");
    }

    Matrix result = Matrix(m, other.n);

    for (int i = 0; i < m; i++) {
        for (int j = 0; j < other.n; j++) {
            for (int k = 0; k < n; k++) {
                result[i][j] += data[i][k] * other[k][j];
            }
        }
    }
    return result;        
}

Matrix::operator std::string () const {
    std::string result;
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            result += std::to_string(data[i][j]) + " ";
        }
        result += "\n";
    }
    return result;
}

float* Matrix::operator[] (int i) {
    return data[i];
}
    
