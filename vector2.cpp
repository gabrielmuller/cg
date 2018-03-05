#include "vector2.h"

Vector2::Vector2 () : Vector2::Vector2(0, 0) {}
  
Vector2::Vector2 (float x, float y) {
    this->x = x;
    this->y = y;
}

Vector2::operator std::string () const {
    return "(" + std::to_string(x) + ", " + std::to_string(y) + ")";
}
