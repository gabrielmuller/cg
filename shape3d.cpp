#include "shape3d.h"

Shape3D::Shape3D(std::string name) : name(name) {}
void Shape3D::translate (Vector3 distance) {
    Transformation t = Transformation::translation3D(distance);
    transform(t);
}
void Shape3D::scale (Vector3 amount) {
    Transformation t = Transformation::scaling3D(amount, center());
    transform(t);
}

void Shape3D::rotate (Rotation rot) {
    Transformation t = Transformation::rotation3D(rot);
    if (rot.no_center) {
        rot.center = center();
    }
    transform(t);
}
