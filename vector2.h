#ifndef VECTOR2_H
#define VECTOR2_H

#include <string>
#include "transformation.h"

struct Vector2 : Transformation {
    float x() const;
    float y() const;

    Vector2();
    Vector2 (float x, float y); 
    Vector2 (const Vector3& other); 
    Vector2 (const Transformation& other); 

    operator std::string () const;
    Vector2 operator+ (Vector2 other) const;
    Vector2 operator- (Vector2 other) const;
    Vector2 operator* (float scalar) const;
    Vector2 operator/ (float scalar) const;
    bool operator== (Vector2 other) const;

    static Vector2 lerp (Vector2 a, Vector2 b, float p);

};

#endif
