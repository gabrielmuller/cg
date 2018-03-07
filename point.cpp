#include "point.h"

Point::Point (std::string name, Vector2 position) : Shape::Shape(name, position) {}
void Point::draw (cairo_t* cr) {
    Vector2 coords = Window::world_to_screen(this->position);
    cairo_move_to(cr, coords.x - 1, coords.y);
    cairo_line_to(cr, coords.x + 1, coords.y);
    cairo_stroke(cr);
}
