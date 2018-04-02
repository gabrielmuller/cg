#include "polygon.h"

Polygon::Polygon (std::string name, std::list<Vector2> verts) : 
    open(false),
    fill(false),
    verts(verts),
    Shape::Shape(name) {}

Polygon::Polygon (std::string name) :
    open(false),
    fill(false),
    Shape::Shape(name) {}

Polygon::Polygon (std::string name, bool fill) :
    open(false),
    fill(fill),
    Shape::Shape(name) {}

void Polygon::draw (cairo_t* cr) {
    if (verts.empty()) {
        return;
    }
    auto it = verts.begin();

    Vector2 coords = Window::world_to_screen(*it);
    cairo_move_to(cr, coords.x(), coords.y());

    for (; it != verts.end(); ++it) {
        coords = Window::world_to_screen(*it);
        cairo_line_to(cr, coords.x(), coords.y());
    }
    if (!open) {
        coords = Window::world_to_screen(verts.front());
        cairo_line_to(cr, coords.x(), coords.y());
    }
    if (fill) cairo_fill(cr);
    cairo_stroke(cr);
}

void Polygon::transform(const Transformation& t) {
    for (auto it = verts.begin(); it != verts.end(); ++it) {
        t.transform(*it);
    }
}

/**
 * @brief       Retorna o centro geométrico do polígono.
 */
Vector2 Polygon::center() const {
    float dx = 0;
    float dy = 0;
    for (auto it = verts.begin(); it != verts.end(); ++it) {
        dx += it->x();
        dy += it->y();
    } 

    // pra não dar problema quando o primeiro é igual
    // ao último vértice do polígono
    // deus me perdoe
    float size = verts.size();
    if (verts.back().x() == verts.front().x() &&
    verts.back().y() == verts.front().y() &&
    size > 1) {
        dx -= verts.front().x();
        dy -= verts.front().y();
        size--;
    }
    
    dx /= size;
    dy /= size;

    return Vector2(dx, dy);
}

    
