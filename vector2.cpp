#include "vector2.h"
#include <iostream>

Vector2::Vector2 () : Vector2::Vector2(0, 0) {}
  
Vector2::Vector2 (float x, float y) : Matrix(1, 3) {
    data[0][0] = x;
    data[0][1] = y;
    data[0][2] = 1;
}

float& Vector2::x () {
    return data[0][0];
}

float& Vector2::y () {
    return data[0][1];
}

float& Vector2::operator[] (int i) {
    return data[0][i];
}

// translação
Vector2 Vector2::operator+ (Vector2 other) {
    Matrix transformation = Matrix(3, 3);
    for (int i = 0; i < 3; i++) transformation[i][i] = 1;
    for (int i = 0; i < 3; i++) transformation[2][i] = other[i];
    Matrix result = *((Matrix*) this) * transformation;
    return *((Vector2*) &result);
}

Vector2 Vector2::operator- (Vector2 other) {
    Vector2 inverted;
    for (int i = 0; i < 3; i++) inverted[i] = other[i] * -1;
    return *(this) + inverted;
}
    
// escalonamento
Vector2 Vector2::operator* (Vector2 other) {
    Matrix transformation = Matrix(3, 3);
    for (int i = 0; i < 3; i++) transformation[i][i] = other[i];
    Matrix result = *((Matrix*) this) * transformation;
    return *((Vector2*) &result);
}
    
