#ifndef POLYGON_H
#define POLYGON_H

#include <math.h>
#include "vertices.h"

struct Polygon : Vertices {
    bool open;
    bool fill;

    Polygon (std::string name, std::vector<Vector2> verts); 
    Polygon (std::string name, std::vector<Vector2> verts, bool fill); 
    Polygon (std::string name); 
    Polygon (std::string name, bool fill); 
    void draw () override;
    void draw_fill ();

};

#endif
