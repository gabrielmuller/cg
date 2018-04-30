#ifndef VECTOR3_H
#define VECTOR3_H

#include <string>

#include "transformation.h"
#include "vector2.h"

struct Vector3 : Transformation {
    float x() const;
    float y() const;
    float z() const;

    Vector3();
    Vector3 (float x, float y, float z); 
    Vector3 (const Transformation& other); 

    operator Vector2 () const;
    operator std::string () const;
    bool operator== (Vector3 other) const;

    static Vector3 lerp (const Vector3& a, const Vector3& b, float p);

};

#endif
