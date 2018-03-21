#include "shape.h"

Shape::Shape (std::string name) : name(name) {}

/**
 * @brief       Translaciona uma forma a partir de um vetor distância.
 *
 * @param[in]   distance     coordenadas do vetor de translação
 */
void Shape::translate (const Vector2& distance) {
    Transformation* t = Transformation::translation(distance);
    transform(*t);
    delete t;
}

/**
 * @brief       Escalona uma forma a partir dos fatores de escala,
 *              em relação ao centro geométrico.
 *
 * @param[in]   amount       fatores de escala
 */
void Shape::scale (const Vector2& amount) {
    Transformation* t = Transformation::scaling(amount, center());
    transform(*t);
    delete t;
}

/**
 * @brief       Rotaciona uma forma em relação a um ponto arbitrário.
 *
 * @param[in]   center       coordenadas do ponto de rotação
 *              rad          graus de rotação
 */
void Shape::rotate (float rad, const Vector2& center) {
    Transformation* t = Transformation::rotation(rad, center);
    transform(*t);
    delete t;
}

/**
 * @brief       Rotaciona uma forma em relação ao centro geométrico.
 *
 * @param[in]   rad     graus de rotação
 */
void Shape::rotate (float rad) {
    rotate(rad, center());
}
