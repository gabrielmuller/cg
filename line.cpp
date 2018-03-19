#include "line.h"

Line::Line (std::string name, Vector2 position,
    Vector2 a, Vector2 b) : Polygon(name, position) {
        verts.push_back(a);
        verts.push_back(b);
}

