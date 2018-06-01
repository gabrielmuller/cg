#include "polyhedron.h"

Polyhedron::Polyhedron (std::string name, std::vector<Edge3D> edges) :
    edges(edges), Shape3D::Shape3D(name) {}

Polyhedron::Polyhedron (std::string name) :
    Shape3D::Shape3D(name) {}

void Polyhedron::draw () {
    for (auto i : edges) {
        Window::draw_line(i);
    }
}

void Polyhedron::transform (const Transformation& t) {
    /*
    for (auto i : edges) {
        i.a = i.a * t;
        i.b = i.b * t;
    }*/
    for (auto it = edges.begin(); it != edges.end(); ++it) {
        auto i = *it;
        it->a = i.a * t;
        it->b = i.b * t;
    }
}

Vector3 Polyhedron::center () const {
    // esse centro ta meio errado...
    float dx = 0;
    float dy = 0;
    float dz = 0;
    for (auto it : edges) {
        dx += it.a.x() + it.b.x();
        dy += it.a.y() + it.b.y();
        dz += it.a.z() + it.b.z();
    } 

    int size = edges.size() * 2;
    dx /= size;
    dy /= size;
    dz /= size;

    return Vector3(dx, dy, dz);
}
