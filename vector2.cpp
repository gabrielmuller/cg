#include "vector2.h"

Vector2::Vector2 () : Vector2::Vector2(0, 0) {}
  
Vector2::Vector2 (float x, float y) : Transformation(1, 3) {
    matrix = {{x, y, 1}};
}

float Vector2::x() const {
    return matrix[0][0];
}

float Vector2::y() const {
    return matrix[0][1];
}

Vector2::operator std::string () const {
    return "(" + std::to_string(x()) + ", " + std::to_string(y()) + ")";
}

Vector2 Vector2::operator+ (Vector2 other) {
    return Vector2(x() + other.x(), y() + other.y());
}

Vector2 Vector2::operator- (Vector2 other) {
    return Vector2(x() - other.x(), y() - other.y());
}

Vector2 Vector2::operator* (float scalar) {
    return Vector2(x() * scalar, y() * scalar);
}

Vector2 Vector2::operator/ (float scalar) {
    return Vector2(x() / scalar, y() / scalar);
}
