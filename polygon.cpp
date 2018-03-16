#include "polygon.h"

Polygon::Polygon (std::string name, Vector2 position) : Shape::Shape(name, position) {}
void Polygon::draw (cairo_t* cr) {
    auto it = verts.begin();

<<<<<<< Updated upstream
    Vector2 coords = Window::world_to_screen(*it + this->position);
    cairo_move_to(cr, coords.x, coords.y);

    for (; it != this->verts.end(); ++it) {
        coords = Window::world_to_screen(*it + this->position);
        cairo_line_to(cr, coords.x, coords.y);
=======
    Vector2 coords = Window::world_to_screen(*it + position);
    cairo_move_to(cr, coords.x(), coords.y());

    for (; it != verts.end(); ++it) {
        coords = Window::world_to_screen(*it + position);
        cairo_line_to(cr, coords.x(), coords.y());
>>>>>>> Stashed changes
    }
    cairo_stroke(cr);
}

