#include "shape.h"

Shape::Shape (std::string name, Vector2 position) : name(name), position(position) {}

/**
 * @brief       Translaciona um polígono a partir de um vetor (Dx,Dy).
 *
 * @param[in]   dx,dy       coordenadas do vetor de translação
 */
void Shape::translate(const Vector2& distance) {
    Transformation* t = Transformation::translation(distance);
    transform(*t);
    delete t;
}

/**
 * @brief       Escalona um polígono a partir de dois fatores de escala Sx e Sy,
 *              em relação ao centro geométrico, utilizando duas translações.
 *
 * @param[in]   sx,sy       fatores de escala
 */
void Shape::scale(const Vector2& amount) {
    Transformation* t = Transformation::scaling(amount, center());
    transform(*t);
    delete t;
}

/**
 * @brief       Rotaciona um polígono em relação a origem, ao centro geométrico
 *              ou a um ponto arbitrário, utilizando duas translações.
 *
 * @param[in]   dx,dy       coordenadas do ponto de rotação
 *              degrees     graus de rotação
 *              center      true se rotação em relação ao centro geométrico
 */
void Shape::rotate(float rad) {
    Transformation* t = Transformation::rotation(rad, center());
    transform(*t);
    delete t;
}
