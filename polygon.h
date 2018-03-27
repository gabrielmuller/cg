#ifndef POLYGON_H
#define POLYGON_H

#include <list>
#include <math.h>
#include <vector>
#include "vector2.h"
#include "shape.h"

struct Polygon : Shape {
    bool open;
    std::list<Vector2> verts;

    Polygon (std::string name, std::list<Vector2> verts); 
    Polygon (std::string name); 
    void draw (cairo_t* cr) override;

    void transform (const Transformation& t) override;

    Vector2 center() const override;
};

#endif
