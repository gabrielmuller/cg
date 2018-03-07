#ifndef POLYGON_H
#define POLYGON_H

#include <list>
#include "vector2.h"
#include "shape.h"


struct Polygon : Shape {
    std::list<Vector2> verts;
    Polygon (std::string name, Vector2 position); 
    void draw (cairo_t* cr);
};

#endif
