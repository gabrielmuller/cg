#include "polygon.h"

Polygon::Polygon (std::string name, std::list<Vector2> verts) : 
    Shape::Shape(name, verts) {}

Polygon::Polygon (std::string name) :
    Shape::Shape(name) {}

void Polygon::draw (cairo_t* cr) {
    auto it = verts.begin();

    Vector2 coords = Window::world_to_screen(*it + position);
    cairo_move_to(cr, coords.x(), coords.y());

    for (; it != verts.end(); ++it) {
        coords = Window::world_to_screen(*it + position);
        cairo_line_to(cr, coords.x(), coords.y());
    }
    cairo_stroke(cr);
}

/**
 * @brief       Retorna o centro geométrico do polígono.
 */
Vector2 Polygon::getCenter() {
    float cx = 0;
    float cy = 0;
    for (auto it = verts.begin(); it != verts.end(); ++it) {
        cx += it->x;
        cy += it->y;
    } 
    cx = cx/(float)(verts.size());
    cy = cy/(float)(verts.size());
    return Vector2(cx, cy);
}

/**
 * @brief       Realiza transformação 2D dos pontos do polígono multiplicando-os
 *              pela matriz de transformação.
 *              Obs: definida apenas pra matriz 3x3.
 *
 * @param[in]   matrix      matriz 3x3 de transformação
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

    
