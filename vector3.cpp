#include "vector3.h"

Vector3::Vector3 () : Vector3::Vector3(0, 0, 0) {}
  
Vector3::Vector3 (float x, float y, float z) : Transformation(1, 4) {
    // teste
    matrix = {{x, y, z, 1}};
}

Vector3::Vector3 (const Transformation& other) : Transformation(1, 4) {
    matrix[0] = other.matrix[0];
}

float Vector3::x() const {
    return matrix[0][0];
}

float Vector3::y() const {
    return matrix[0][1];
}

float Vector3::z() const {
    return matrix[0][2];
}

Vector3::operator std::string () const {
    return "(" + std::to_string(x()) + ", " 
        + std::to_string(y())
        + std::to_string(z()) + ")";
}

bool Vector3::operator== (Vector3 other) const {
    return x() == other.x()  
        && y() == other.y()
        && z() == other.z();
}

    
