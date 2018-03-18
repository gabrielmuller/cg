#include "vector2.h"
#include <stdexcept>
#include <iostream>

Vector2::Vector2 () : Vector2::Vector2(0, 0) {}
  
Vector2::Vector2 (float x, float y) : Matrix(1, 3) {
    data[0][0] = x;
    data[0][1] = y;
    data[0][2] = 1;
}

Vector2::Vector2 (const Vector2& other) : Matrix::Matrix(other) {}
Vector2::Vector2 (const Matrix& other) : Matrix(1, 3) {
    if (other.m != 1 || other.n != 3) {
        std::cout << other.n << " " << other.m;
        throw new std::domain_error("Matriz não é um Vector2");
    }
    for (int i = 0; i < 3; i++) {
        data[0][i] = other[0][i];
    }
}

float& Vector2::x () {
    return data[0][0];
}

float& Vector2::y () {
    return data[0][1];
}

float& Vector2::operator[] (int i) const {
    return data[0][i];
}

// translação
Vector2 Vector2::operator+ (Vector2 other) const {
    Matrix transformation (3, 3);
    for (int i = 0; i < 3; i++) transformation[i][i] = 1;
    for (int i = 0; i < 3; i++) transformation[2][i] = other[i];
    return Vector2(Matrix(*this) * transformation);
}

Vector2 Vector2::operator- (Vector2 other) const {
    Vector2 inverted;
    for (int i = 0; i < 2; i++) inverted[i] = other[i] * -1;
    return *this + inverted;
}
    
// escalonamento
Vector2 Vector2::operator* (Vector2 other) const {
    Matrix transformation (3, 3);
    for (int i = 0; i < 3; i++) transformation[i][i] = other[i];
    return Vector2(Matrix(*this) * transformation);
}
