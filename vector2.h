#ifndef VECTOR2_H
#define VECTOR2_H

#include <string>
#include "matrix.h"

struct Vector2 : Matrix {

    Vector2();
    Vector2 (float x, float y); 
    Vector2 (const Vector2& other);
    Vector2 (const Matrix& other);

    float& x ();
    float& y ();

    Vector2 operator+ (Vector2 other) const;
    Vector2 operator- (Vector2 other) const;
    Vector2 operator* (Vector2 other) const;
    float& operator[] (int i) const;

};

#endif
