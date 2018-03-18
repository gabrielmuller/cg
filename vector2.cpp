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

Vector2 Vector2::operator* (float scalar) {
    return Vector2(this->x * scalar, this->y * scalar);
}

Vector2 Vector2::operator/ (float scalar) {
    return Vector2(this->x / scalar, this->y / scalar);
}
