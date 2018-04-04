#include "vertices.h"

Vertices::Vertices (std::string name, std::vector<Vector2> verts) : 
    verts(verts),
    Shape::Shape(name) {}

Vertices::Vertices (std::string name) : Shape::Shape(name) {}

void Vertices::transform(const Transformation& t) {
    for (auto it = verts.begin(); it != verts.end(); ++it) {
        t.transform(*it);
    }
}

/**
 * @brief       Retorna o centro geométrico da figura.
 */
Vector2 Vertices::center() const {
    float dx = 0;
    float dy = 0;
    for (auto it = verts.begin(); it != verts.end(); ++it) {
        dx += it->x();
        dy += it->y();
    } 

    int size = verts.size();
    dx /= size;
    dy /= size;

    return Vector2(dx, dy);
}
