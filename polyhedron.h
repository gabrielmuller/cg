#ifndef POLYHEDRON_H
#define POLYHEDRON_H

#include <vector>
#include "edge3d.h"
#include "shape3d.h"
#include "transformation.h"

struct Polyhedron : Shape3D {
    std::vector<Edge3D> edges;

    Polyhedron (std::string name, std::vector<Edge3D> edges);

    void draw ();
    void transform (Transformation t);
    void translate (Vector3 distance);
    void scale (Vector3 amount);
    void rotate (Vector3 angles, Vector3 center);
    void rotate (Vector3 angles);
    Vector3 center () const;
};

#endif
