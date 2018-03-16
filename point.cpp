#include "point.h"

Point::Point (std::string name, Vector2 position) : Shape::Shape(name, position) {}
void Point::draw (cairo_t* cr) {
<<<<<<< Updated upstream
    Vector2 coords = Window::world_to_screen(this->position);
    cairo_move_to(cr, coords.x - 1, coords.y);
    cairo_line_to(cr, coords.x + 1, coords.y);
=======
    Vector2 coords = Window::world_to_screen(position);
    cairo_move_to(cr, coords.x() - 1, coords.y());
    cairo_line_to(cr, coords.x() + 1, coords.y());
>>>>>>> Stashed changes
    cairo_stroke(cr);
}
