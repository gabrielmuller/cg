#ifndef POINT_H
#define POINT_H

#include <list>
#include <vector>
#include "vector2.h"
#include "shape.h"

struct Point : Shape {
    Point (std::string name, Vector2 position); 
    void draw (cairo_t* cr);
    void translation (float dx, float dy);
    void scaling(float sx, float sy);
    void rotation(float dx, float dy, float degrees, bool center);
};

#endif
