#include "polyhedron3d.h"
#include "window.h"

Polyhedron::Polyhedron (std::string name, std::vector<Edge3D> edges) :
    edges(edges), Shape3D::Shape3D(name) {}


void Polyhedron::draw () {
    for (auto i : edges) {
        Window::draw(*i);
    }
}

void Polyhedron::transform (Transformation t) {}
void Polyhedron::translate (Vector3 distance) {}
void Polyhedron::scale (Vector3 amount) {}
void Polyhedron::rotate (Vector3 angles, Vector3 center) {}
void Polyhedron::rotate (Vector3 angles) {}
Vector3 Polyhedron::center () const {}
