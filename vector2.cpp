#include "vector2.h"
#include "vector3.h"

Vector2::Vector2 () : Vector2::Vector2(0, 0) {}
  
Vector2::Vector2 (float x, float y) : Transformation(1, 3) {
    matrix = {{x, y, 1}};
}

Vector2::Vector2 (const Transformation& other) : Transformation(1, 3) {
    matrix[0] = other.matrix[0];
}

Vector2::Vector2 (const Vector3& other) : Vector2(other.x(), other.y()) {}

float Vector2::x() const {
    return matrix[0][0];
}

float Vector2::y() const {
    return matrix[0][1];
}

Vector2::operator std::string () const {
    return "(" + std::to_string(x()) + ", " + std::to_string(y()) + ")";
}

Vector2 Vector2::operator+ (Vector2 other) const {
    Transformation t = Transformation::translation(other);
    Vector2 result = (Transformation) *this * t;
    return result;
}

Vector2 Vector2::operator- (Vector2 other) const {
    other.matrix[0][0] *= -1;
    other.matrix[0][1] *= -1;
    return *this + other;
}

Vector2 Vector2::operator* (float scalar) const {
    Transformation t = Transformation::scaling(Vector2(scalar, scalar), Vector2(0, 0));
    Vector2 result = (Transformation) *this * t;
    return result;
}

Vector2 Vector2::operator/ (float scalar) const {
    return *this * (1/scalar);
}

Vector2 Vector2::lerp (Vector2 a, Vector2 b, float p) {
    p = p > 1 ? 1 : p;
    p = p < 0 ? 0 : p;
    return (a * (1-p) + b * p);
}

bool Vector2::operator== (Vector2 other) const {
    return x() == other.x()  && y() == other.y();
}

    
