#ifndef POLYGON_H
#define POLYGON_H

#include <list>
#include <math.h>
#include <vector>
#include "vector2.h"
#include "shape.h"

struct Polygon : Shape {
    Polygon (std::string name, std::list<Vector2> verts); 
    Polygon (std::string name); 
    void draw (cairo_t* cr);

    Vector2 getCenter();

    void transform(std::vector<std::vector<float>> matrix);
    void translation(float dx, float dy);
    void scaling(float sx, float sy);
    void rotation(float dx, float dy, float degrees, bool center);
};

#endif
