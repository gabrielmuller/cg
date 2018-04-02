#include "line.h"

Line::Line (std::string name, std::list<Vector2> verts) : 
	Polygon(name, verts) {
        open = true;
}

void Line::draw (cairo_t* cr) {
    AB line (verts.front(), verts.back());
    line = Window::clip_line(line);
    line = AB (Window::world_to_screen(line.a), Window::world_to_screen(line.b));
    cairo_move_to(cr, line.a.x(), line.a.y());
    cairo_line_to(cr, line.b.x(), line.b.y());
    cairo_stroke(cr);
}

