#ifndef VECTOR2_H
#define VECTOR2_H

#include <string>
#include "matrix.h"

struct Vector2 : Matrix {

    Vector2();
    Vector2 (float x, float y); 

    float& x ();
    float& y ();

    Vector2 operator+ (Vector2 other);
    Vector2 operator- (Vector2 other);
    Vector2 operator* (Vector2 other);
    float& operator[] (int i);

};

#endif
