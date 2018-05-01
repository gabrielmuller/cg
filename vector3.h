#ifndef VECTOR3_H
#define VECTOR3_H

#include <string>
#include "transformation.h"

struct Vector3 : Transformation {
    float x() const;
    float y() const;
    float z() const;

    float length() const;
    
    Vector3();
    Vector3 (float x, float y, float z); 
    Vector3 (const Transformation& other); 
    Vector3 normalized() const;

    operator std::string () const;
    bool operator== (Vector3 other) const;

    static Vector3 lerp (const Vector3& a, const Vector3& b, const float p);

};

#endif
