#include "point.h"

Point::Point (std::string name, float x, float y) : 
    Shape::Shape(name) {
        position = Vector2(x, y);
    }

void Point::draw () {
    Window::draw_point(position);
}

void Point::transform (const Transformation& t) {
    t.transform(position);
}

Vector2 Point::center () const {
    return position;
}
