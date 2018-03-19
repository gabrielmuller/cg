#include "polygon.h"
#include <iostream>

Polygon::Polygon (std::string name, Vector2 position) : Shape::Shape(name, position) {}

void Polygon::draw (cairo_t* cr) {
    auto it = this->verts.begin();

    Vector2 coords = Window::world_to_screen(*it + this->position);
    cairo_move_to(cr, coords.x, coords.y);

    for (; it != this->verts.end(); ++it) {
        coords = Window::world_to_screen(*it + this->position);
        cairo_line_to(cr, coords.x, coords.y);
    }
    cairo_stroke(cr);
}

void Polygon::transform(
        std::vector<std::vector<float>> matrix) {
    for (auto it = this->verts.begin(); it != this->verts.end(); ++it) {
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
}

void Polygon::translation(float dx, float dy) {
    std::vector<std::vector<float>> transform_matrix;
    transform_matrix = {{1,0,0},{0,1,0},{dx,dy,1}};
    transform(transform_matrix);
}

void Polygon::scaling(float sx, float sy) {
    float cx, cy = 0;
    for (auto it = this->verts.begin(); it != this->verts.end(); ++it) {
        cx += it->x;
        cy += it->y;
    } 
    cx = cx/(float)(this->verts.size()-1); //??
    cy = cy/(float)(this->verts.size()-1); //??
    std::vector<std::vector<float>> scale_matrix, t1_matrix, t2_matrix;
    t1_matrix = {{1,0,0},{0,1,0},{-cx,-cy,1}};
    scale_matrix = {{sx,0,0},{0,sy,0},{0,0,1}};
    t2_matrix = {{1,0,0},{0,1,0},{cx,cy,1}};
    transform(t1_matrix);
    transform(scale_matrix);
    transform(t2_matrix);
}

// Precisa ajustar
// Mover pro centro mover de volta sei la o que
void Polygon::rotation(float degrees) {
    float radian = (M_PI/180)*degrees;
    std::vector<std::vector<float>> transform_matrix;
    transform_matrix = {{std::cos(radian),-std::sin(radian),0},{std::sin(radian),std::cos(radian),0},{0,0,1}};
    transform(transform_matrix);
}

