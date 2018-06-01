#ifndef EDGE3D_H
#define EDGE3D_H

#include "vector3.h"

struct Edge3D {
    Vector3 a;
    Vector3 b;

    Edge3D ();
    Edge3D (Vector3 a, Vector3 b);
};

#endif
