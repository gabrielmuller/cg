#include "polygon.h"

Polygon::Polygon (std::string name, std::list<Vector2> verts) : 
    verts(verts),
    Shape::Shape(name) {}

Polygon::Polygon (std::string name) :
    Shape::Shape(name) {}

void Polygon::draw () {
    if (verts.empty()) {
        return;
    }
    auto it = verts.begin();

    //Vector2 coords = Window::world_to_screen(*it);
    //cairo_move_to(cr, coords.x(), coords.y());
    Vector2 pos = *it;

    for (; it != verts.end(); ++it) {
        /*cairo_move_to(cr, coords.x(), coords.y());
        coords = Window::world_to_screen(*it);
        cairo_line_to(cr, coords.x(), coords.y());*/
        AB edge (pos, *it);
        Window::draw_line(edge);
        pos = *it;
    }
    if (!open) {
        /*coords = Window::world_to_screen(verts.front());
        cairo_line_to(cr, coords.x(), coords.y());*/
        AB edge (verts.back(), verts.front());
        Window::draw_line(edge);
    }
    //cairo_stroke(cr);
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

    
