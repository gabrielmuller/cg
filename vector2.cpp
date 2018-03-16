#include "vector2.h"

Vector2::Vector2 () : Vector2::Vector2(0, 0) {}
  
Vector2::Vector2 (float x, float y) {
    this->x = x;
    this->y = y;
}

Vector2::operator std::string () const {
    return "(" + std::to_string(x) + ", " + std::to_string(y) + ")";
}

Vector2 Vector2::operator+ (Vector2 other) {
    return Vector2(this->x + other.x, this->y + other.y);
}

Vector2 Vector2::operator- (Vector2 other) {
    return Vector2(this->x - other.x, this->y - other.y);
}

<<<<<<< Updated upstream
Vector2 Vector2::operator* (float scalar) {
    return Vector2(this->x * scalar, this->y * scalar);
}

Vector2 Vector2::operator/ (float scalar) {
    return Vector2(this->x / scalar, this->y / scalar);
=======
// translação
Vector2 Vector2::operator+ (Vector2 other) {
    Matrix transformation = Matrix(3, 3);
    other[2] = 1;
    for (int i = 0; i < 3; i++) transformation[i][i] = 1;
    for (int i = 0; i < 3; i++) transformation[2][i] = other[i];
    std::cout << (std::string) transformation;
    Matrix result = *((Matrix*) this) * transformation;
    return *((Vector2*) &result);
}

Vector2 Vector2::operator- (Vector2 other) {
    Vector2 inverted;
    for (int i = 0; i < 2; i++) inverted[i] = other[i] * -1;
    inverted[2] = other[2];
    return *(this) + inverted;
}
    
// escalonamento
Vector2 Vector2::operator* (Vector2 other) {
    Matrix transformation = Matrix(3, 3);
    other[2] = 1;
    for (int i = 0; i < 3; i++) transformation[i][i] = other[i];
    Matrix result = *((Matrix*) this) * transformation;
    return *((Vector2*) &result);
>>>>>>> Stashed changes
}
