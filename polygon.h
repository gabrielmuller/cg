#ifndef POLYGON_H
#define POLYGON_H

#include <list>
#include <math.h>
#include <vector>
#include "vector2.h"
#include "shape.h"

struct Polygon : Shape {
    bool open;
    bool fill;
    std::list<Vector2> verts;

    Polygon (std::string name, std::list<Vector2> verts); 
    Polygon (std::string name); 
    Polygon (std::string name, bool fill); 
    void draw () override;
    void draw_fill ();

    void transform (const Transformation& t) override;

    AB clip_to_edge(AB edge, AB line);
    bool is_inside(Vector2 coord, AB edge);
    Vector2 center() const override;
};

#endif
