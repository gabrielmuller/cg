#include "vector2.h"

Vector2::Vector2 () : Vector2::Vector2(0, 0) {}
  
Vector2::Vector2 (float x, float y) : x(x), y(y) {}

Vector2::operator std::string () const {
    return "(" + std::to_string(x) + ", " + std::to_string(y) + ")";
}

Vector2 Vector2::operator+ (Vector2 other) {
    return Vector2(x + other.x, y + other.y);
}

Vector2 Vector2::operator- (Vector2 other) {
    return Vector2(x - other.x, y - other.y);
}

Vector2 Vector2::operator* (float scalar) {
    return Vector2(x * scalar, y * scalar);
}

Vector2 Vector2::operator/ (float scalar) {
    return Vector2(x / scalar, y / scalar);
}
