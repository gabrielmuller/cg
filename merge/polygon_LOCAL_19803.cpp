#include "polygon.h"

Polygon::Polygon (std::string name, Vector2 position) : Shape::Shape(name, position) {}

void Polygon::draw (cairo_t* cr) {
    auto it = verts.begin();

    Vector2 coords = Window::world_to_screen(*it + position);
    cairo_move_to(cr, coords.x(), coords.y());

    if (name == "Square") {
        ;
    }

    for (; it != verts.end(); ++it) {
        coords = Window::world_to_screen(*it + position);
        cairo_line_to(cr, coords.x(), coords.y());
    }
    cairo_stroke(cr);
}

/**
 * @brief       Realiza transformação 2D dos pontos do polígono multiplicando-os
 *              pela matriz de transformação.
 *
 * @param[in]   matrix      matriz de transformação
 */
void Polygon::transform(const Transformation& t) {
    for (auto it = verts.begin(); it != verts.end(); ++it) {
        t.transform(*it);
    }
}

Vector2 Polygon::center() const {
    float dx = 0;
    float dy = 0;
    for (auto it = verts.begin(); it != this->verts.end(); ++it) {
        dx += it->x();
        dy += it->y();
    } 
    dx /= (float)(verts.size()-1); //??
    dy /= (float)(this->verts.size()-1); //??

    return Vector2(dx, dy);
}

    
