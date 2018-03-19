#include "polygon.h"

Polygon::Polygon (std::string name, Vector2 position) : Shape::Shape(name, position) {}

void Polygon::draw (cairo_t* cr) {
    auto it = verts.begin();

    Vector2 coords = Window::world_to_screen(*it + position);
    cairo_move_to(cr, coords.x, coords.y);

    for (; it != verts.end(); ++it) {
        coords = Window::world_to_screen(*it + position);
        cairo_line_to(cr, coords.x, coords.y);
    }
    cairo_stroke(cr);
}

/**
 * @brief       Realiza transformação 2D dos pontos do polígono multiplicando-os
 *              pela matriz de transformação.
 *
 * @param[in]   matrix      matriz de transformação
 */
void Polygon::transform(
        std::vector<std::vector<float>> matrix) {
    for (auto it = verts.begin(); it != verts.end(); ++it) {
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

/**
 * @brief       Translaciona um polígono a partir de um vetor (Dx,Dy).
 *
 * @param[in]   dx,dy       coordenadas do vetor de translação
 */
void Polygon::translation(float dx, float dy) {
    std::vector<std::vector<float>> transform_matrix;
    transform_matrix = {{1,0,0},{0,1,0},{dx,dy,1}};
    transform(transform_matrix);
}

/**
 * @brief       Escalona um polígono a partir de dois fatores de escala Sx e Sy,
 *              em relação ao centro geométrico, utilizando duas translações.
 *
 * @param[in]   sx,sy       fatores de escala
 */
void Polygon::scaling(float sx, float sy) {
    float cx, cy = 0;
    for (auto it = this->verts.begin(); it != this->verts.end(); ++it) {
        cx += it->x;
        cy += it->y;
    } 
    cx = cx/(float)(this->verts.size()-1); //??
    cy = cy/(float)(this->verts.size()-1); //??
    std::vector<std::vector<float>> scale_matrix;
    scale_matrix = {{sx,0,0},{0,sy,0},{0,0,1}};
    translation(-cx, -cy);
    transform(scale_matrix);
    translation(cx, cy);
}

/**
 * @brief       Rotaciona um polígono em relação a origem, ao centro geométrico
 *              ou a um ponto arbitrário, utilizando duas translações.
 *
 * @param[in]   dx,dy       coordenadas do ponto de rotação
 *              degrees     graus de rotação
 *              center      true se rotação em relação ao centro geométrico
 */
void Polygon::rotation(float dx, float dy, float degrees, bool center) {
    if(center) {
        dx, dy = 0;
        for (auto it = this->verts.begin(); it != this->verts.end(); ++it) {
            dx += it->x;
            dy += it->y;
        } 
        dx = dx/(float)(this->verts.size()-1); //??
        dy = dy/(float)(this->verts.size()-1); //??
    }
    float radian = (M_PI/180)*degrees;
    std::vector<std::vector<float>> transform_matrix;
    transform_matrix = {{std::cos(radian),-std::sin(radian),0},
        {std::sin(radian),std::cos(radian),0},{0,0,1}};
    translation(-dx,-dy);
    transform(transform_matrix);
    translation(dx,dy);
}

