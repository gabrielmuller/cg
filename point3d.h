#ifndef POINT3D_H
#define POINT3D_H

#include <list>
#include <vector>
#include "vector3.h"
#include "shape3d.h"

struct Point3D : Shape3D {
    Vector3 position;

    Point3D (std::string name, float x, float y, float z); 
    void draw () override;

    void transform (const Transformation& t) override;
    void translate (const Vector3& distance);
    void scale (const Vector3& amount);
    void rotate (Rotation rot);
    Vector3 center () const override;
};

#endif
