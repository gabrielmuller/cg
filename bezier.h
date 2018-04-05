#ifndef BEZIER_H
#define BEZIER_H

#include "polygon.h"

struct Bezier : Vertices {
    Bezier(std::string name, std::vector<Vector2> verts);
    void draw ();

    private:
    float t_times(std::vector<float> v, float t);
};
#endif
