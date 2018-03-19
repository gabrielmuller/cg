#include <stdexcept>
#include "matrix.h"


Matrix::Matrix (int sizeX, int sizeY) {
    this->sizeX = sizeX;
    this->sizeY = sizeY;
    data = new float* [sizeY];
    for (int i = 0; i < sizeY; i++) {
       data[i] = new float [sizeX];
    }
}
        
    
Matrix Matrix::multiply (Matrix a, Matrix b) {
    if (a.sizeY != b.sizeX) {
        throw new std::domain_error("Tamanhos inválidos de matriz");
    }

    Matrix result = Matrix(a.sizeX, b.sizeY);

    for (int i = 0; i < a.sizeY; i++) {
        for (int j = 0; j < b.sizeX; j++) {
            for (int k = 0; k < a.sizeX; k++) {
                result[i][j] += a[i][k] * b[k][j];
            }
        }
    }
    return result;        
}

Matrix::operator std::string () const {
    std::string result;
    for (int i = 0; i < sizeX; i++) {
        for (int j = 0; j < sizeX; j++) {
            result += std::to_string(data[i][j]) + " ";
        }
        result += "\n";
    }
    return result;
}

float* Matrix::operator[] (int i) {
    return data[i];
}
    