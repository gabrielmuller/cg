#include "point3d.h"

Point3D::Point3D (std::string name, float x, float y, float z) : 
    Shape3D::Shape3D(name) {
        position = Vector3(x, y, z);
    }

void Point3D::draw () {
    Window::draw_point(position);
}

void Point3D::transform (const Transformation& t) {
    t.transform(position);
}

Vector3 Point3D::center () const {
    return position;
}