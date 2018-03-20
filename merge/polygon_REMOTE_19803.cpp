#include "polygon.h"
#include <iostream>

Polygon::Polygon (std::string name, std::list<Vector2> verts) : 
    Shape::Shape(name, verts) {}

Polygon::Polygon (std::string name) :
    Shape::Shape(name) {}

void Polygon::draw (cairo_t* cr) {
    auto it = verts.begin();

    Vector2 coords = Window::world_to_screen(*it);
    cairo_move_to(cr, coords.x, coords.y);

    for (; it != verts.end(); ++it) {
        coords = Window::world_to_screen(*it);
        cairo_line_to(cr, coords.x, coords.y);
        // Definir sem repetir o ponto inicial:
        /*if (it == prev(verts.end())) {
            coords = verts.front();
            coords = Window::world_to_screen(coords);
            cairo_line_to(cr, coords.x, coords.y);
        }*/
    }
    cairo_stroke(cr);
}

/**
 * @brief       Retorna o centro geométrico do polígono.
 */
Vector2 Polygon::getCenter() {
    float cx = 0;
    float cy = 0;
    for (auto it = verts.begin(); it != verts.end(); ++it) {
        cx += it->x;
        cy += it->y;
    } 
    cx = cx/(float)(verts.size());
    cy = cy/(float)(verts.size());
    return Vector2(cx, cy);
}

/**
 * @brief       Realiza transformação 2D dos pontos do polígono multiplicando-os
 *              pela matriz de transformação.
 *              Obs: definida apenas pra matriz 3x3.
 *
 * @param[in]   matrix      matriz 3x3 de transformação
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
    float cx = getCenter().x;
    float cy = getCenter().y;
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
        dx = getCenter().x;
        dy = getCenter().y;
    }
    float radian = (M_PI/180)*degrees;
    std::vector<std::vector<float>> transform_matrix;
    transform_matrix = {{(float)cos(radian),(float)(-sin(radian)),0},
         {(float)sin(radian),(float)cos(radian),0},{0,0,1}};
    translation(-dx,-dy);
    transform(transform_matrix);
    translation(dx,dy);
}

