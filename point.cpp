#include "point.h"

Point::Point (std::string name, std::list<Vector2> verts) : 
    Shape::Shape(name, verts) {}

void Point::draw (cairo_t* cr) {
    auto it = verts.begin();
    Vector2 coords = Window::world_to_screen(*it);
    cairo_move_to(cr, coords.x - 1, coords.y);
    cairo_line_to(cr, coords.x + 1, coords.y);
    cairo_stroke(cr);
}

void Point::translation (float dx, float dy) {
	std::vector<std::vector<float>> matrix;
    matrix = {{1,0,0},{0,1,0},{dx,dy,1}};

    auto it = verts.begin();
	std::vector<float> old_coord = {it->x, it->y, 1};
    std::vector<float> new_coord(3);

    for (int i = 0; i < 3; ++i) {
        new_coord[i] = 0;
        for (int j = 0; j < 3; ++j) {
            new_coord[i] += old_coord[j] * matrix[j][i];
        }
    }
    it->x = new_coord[0];
    it->y = new_coord[1];    

}

void Point::scaling(float sx, float sy) {}

void Point::rotation(float dx, float dy, float degrees, bool center) {}

