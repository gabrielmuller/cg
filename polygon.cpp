#include "polygon.h"

Polygon::Polygon (std::string name, Vector2 position) : Shape::Shape(name, position) {}
void Polygon::draw (cairo_t* cr) {
    auto it = this->verts.begin();

    Vector2 coords = Window::world_to_screen(*it + this->position);
    cairo_move_to(cr, coords.x(), coords.y());

    for (; it != this->verts.end(); ++it) {
        coords = Window::world_to_screen(*it + this->position);
        cairo_line_to(cr, coords.x(), coords.y());
    }
    cairo_stroke(cr);
}

