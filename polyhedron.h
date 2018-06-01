#ifndef POLYHEDRON_H
#define POLYHEDRON_H

#include <vector>
#include "edge3d.h"
#include "shape3d.h"
#include "transformation.h"

struct Polyhedron : Shape3D {
    std::vector<Edge3D> edges;

    Polyhedron (std::string name, std::vector<Edge3D> edges);
    Polyhedron (std::string name);

    void draw () override;
    void transform (const Transformation& t) override;
    Vector3 center () const override;
};

#endif
