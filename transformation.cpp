#include "transformation.h"
#include <stdexcept>

Transformation::Transformation (int m, int n) : m(m), n(n) {
    matrix = std::vector<std::vector<float>> (m);
    for (int i = 0; i < m; i++) {
        matrix[i] = std::vector<float> (n);
    }
}

Transformation* Transformation::operator* (const Transformation& t) const {
    if (n != t.m) {
        throw new std::domain_error("Tamanhos inválidos de matriz");
    }
    Transformation* result = new Transformation(m, t.n);
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < t.n; j++) {
            result->matrix[i][j] = 0;
            for (int k = 0; k < n; k++) {
                result->matrix[i][j] += matrix[i][k] * t.matrix[k][j];
            }
        }
    }
    return result;
}

void Transformation::transform (Transformation*& t) const {
    t = *t * *this;
}

Transformation::operator std::string () const {
    std::string result;
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            result += std::to_string(matrix[i][j]) + " ";
        }
        result += "\n";
    }
    return result;
}
    
        

