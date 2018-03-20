#include "vector2.h"
#include <stdexcept>
#include "math.h"

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

Transformation* Transformation::translation (const Vector2& distance) {
    Transformation* t = new Transformation (3, 3);
    t->matrix = {{1,0,0},{0,1,0},{distance.x(), distance.y(), 1}};
    return t;
}

Transformation* Transformation::scaling (const Vector2& amount, const Vector2& center) {
    Transformation* t = new Transformation (3, 3);
    // deslocamento pro centro + escalonamento pré-calculado
    t->matrix = {
        {amount.x(), 0, 0},
        {0, amount.y(), 0},
        {center.x() * (-amount.x() + 1), center.y() * (-amount.y() + 1), 1} 
    };
    return t;
}

Transformation* Transformation::rotation (const float rad, const Vector2& center) {
    Transformation* t = new Transformation (3, 3);
    //pré calcular trigonometria
    float c = std::cos(rad);
    float s = std::sin(rad);

    //matriz pré calculada
    t->matrix = {
        {c, -s, 0},
        {s,  c, 0},
        {-center.y()*s + center.x()*(1-c), center.x()*s + center.y()*(1-c), 1}
    };
    return t;
}

void Transformation::transform (Transformation& t) const {
    Transformation* mult = t * *this;
    t = *mult;
    delete mult;
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

Transformation& Transformation::operator= (const Transformation& other) {
    m = other.m;
    n = other.n;
    for (int i = 0; i < m; i++) {
        matrix[i] = other.matrix[i];
    }
    return *this;
}
