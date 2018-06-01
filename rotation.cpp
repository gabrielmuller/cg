#include "rotation.h"

Rotation::Rotation(Vector3 center, Vector3 axis, float angle)
    : center(center), axis(axis), angle(angle), no_center(false) {
        axis = axis.normalized();
}

Rotation::Rotation(Vector3 axis, float angle) 
    : axis(axis), angle(angle), no_center(true) {
        axis = axis.normalized();
}
