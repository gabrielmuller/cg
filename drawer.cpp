#include "window.h"
#include "drawer.h"

void Drawer::draw (cairo_t* cr, Point point) {
    Vector2 coords = Window::world_to_screen(point.position);
    cairo_move_to(cr, coords.x, coords.y);
    cairo_stroke(cr);
}

void Drawer::draw (cairo_t* cr, Polygon poly) {
    
    auto it = poly.verts.begin();

    Vector2 coords = Window::world_to_screen(*it + poly.position);
    cairo_move_to(cr, coords.x, coords.y);

    for (; it != poly.verts.end(); ++it) {
        coords = Window::world_to_screen(*it + poly.position);
        cairo_line_to(cr, coords.x, coords.y);
    }
    cairo_stroke(cr);
}
