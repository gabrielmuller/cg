#include "point.h"

Point::Point (std::string name, Vector2 position) : Shape::Shape(name, position) {}

void Point::draw (cairo_t* cr) {
    Vector2 coords = Window::world_to_screen(position);
    cairo_move_to(cr, coords.x - 1, coords.y);
    cairo_line_to(cr, coords.x + 1, coords.y);
    cairo_stroke(cr);
}

void Point::translation (float dx, float dy) {
	std::vector<std::vector<float>> matrix;
    matrix = {{1,0,0},{0,1,0},{dx,dy,1}};

	std::vector<float> old_coord = {position.x, position.y, 1};
    std::vector<float> new_coord(3);

    for (int i = 0; i < 3; ++i) {
        new_coord[i] = 0;
        for (int j = 0; j < 3; ++j) {
            new_coord[i] += old_coord[j] * matrix[j][i];
        }
    }
    position.x = new_coord[0];
    position.y = new_coord[1];    

}

void Point::scaling(float sx, float sy) {}

void Point::rotation(float degree) {}

