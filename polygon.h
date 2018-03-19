#ifndef POLYGON_H
#define POLYGON_H

#include <list>
#include <math.h>
#include <vector>
#include "vector2.h"
#include "shape.h"

struct Polygon : Shape {
    std::list<Vector2> verts;
    Polygon (std::string name, Vector2 position); 
    void draw (cairo_t* cr);

    void transform(
    	std::vector<std::vector<float>> matrix);
    void translation(float dx, float dy);
    void scaling(float sx, float sy);
    void rotation(float dx, float dy, float degrees, bool center);
};

#endif
