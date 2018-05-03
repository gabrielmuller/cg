#ifndef ROTATION_H
#define ROTATION_H

#include "vector3.h"

struct Rotation {
    Vector3 center;
    Vector3 axis;
    float angle;
    bool no_center;
    
    Rotation(Vector3 center, Vector3 axis, float angle);
    Rotation(Vector3 axis, float angle);
};


#endif
