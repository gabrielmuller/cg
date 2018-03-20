#include "point.h"

Point::Point (std::string name, std::list<Vector2> verts) : 
    Shape::Shape(name, verts) {}

void Point::draw (cairo_t* cr) {
    Vector2 coords = Window::world_to_screen(position);
    cairo_move_to(cr, coords.x() - 1, coords.y());
    cairo_line_to(cr, coords.x() + 1, coords.y());
    cairo_stroke(cr);
}

void Point::transform (const Transformation& t) {
    t.transform(position);
}

Vector2 Point::center () const {
    return position;
}
