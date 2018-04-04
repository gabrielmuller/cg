#include "bezier.h"
#include "vertices.h"

Bezier::Bezier(std::string name, std::vector<Vector2> verts) 
    : Vertices::Vertices(name, verts) {}

void Bezier::draw () {
    if (verts.empty()) {
        return;
    }
}
