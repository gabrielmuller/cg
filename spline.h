#ifndef SPLINE_H
#define SPLINE_H

#include "polygon.h"

struct Spline : Vertices {
    Spline(std::string name, std::vector<Vector2> verts);
    Spline(std::string name);
    void draw ();

    private:
    void fwdDiff (float n, float x, float dx, float d2x, float d3x, 
        float y, float dy, float d2y, float d3y);
};
#endif
