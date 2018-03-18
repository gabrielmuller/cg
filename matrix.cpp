#include <stdexcept>
#include "matrix.h"
#include <iostream>

Matrix::Matrix (int m, int n) : m(m), n(n) {
    data = new float* [m];
    for (int i = 0; i < m; i++) {
       data[i] = new float [n] {};
    }
}

Matrix::~Matrix () {
    for (int i = 0; i < m; i++) {
        delete[] data[i];
    }
    delete[] data;
}

Matrix::Matrix (const Matrix& other) 
    : Matrix::Matrix(other.m, other.n) {
    for (int i = 0; i < m; i++) {
        // TODO: memcpy
        for (int j = 0; j < n; j++) {
            data[i][j] = other[i][j];
        }
    }
}
        
Matrix Matrix::operator* (Matrix other) {
    if (n != other.m) {
        throw new std::domain_error("Tamanhos inválidos de matriz");
    }

    Matrix result (m, other.n);

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

float* Matrix::operator[] (int i) const {
    if (i < m) {
        return data[i];
    } else {
        throw new std::out_of_range("Matriz não tem tantas linhas.");
    }
}
    
Matrix& Matrix::operator= (Matrix& other) {
    return Matrix(other);
