#ifndef VECTOR2_H
#define VECTOR2_H

#include <string>

#include "transformation.h"

struct Vector2 : Transformation {
    float x() const;
    float y() const;

    Vector2();
    Vector2 (float x, float y); 
    Vector2 (const Transformation& other); 

    operator std::string () const;
    Vector2 operator+ (Vector2 other) const;
    Vector2 operator- (Vector2 other) const;
    Vector2 operator* (float scalar) const;
    Vector2 operator/ (float scalar) const;

    static Vector2 lerp (const Vector2& a, const Vector2& b, float p);

};

#endif
