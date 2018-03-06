#include "camera.h"

Vector2 Camera::position(0, 0);
Vector2 Camera::viewport(400, 400);

// tamanho da visão da cãmera (mundo)
Vector2 Camera::size(10, 10);

// converte uma coordenada do espaço no mundo para tela
Vector2 Camera::world_to_screen(Vector2 coords) {
    Vector2 wmin = Camera::position - (Camera::size / 2);
    Vector2 output(coords - wmin);
    output = Vector2(output.x / Camera::size.x, output.y / Camera::size.y);
    output.y = 1 - output.y;
    output = Vector2(output.x * Camera::viewport.x, output.y * Camera::viewport.y);

    return output;
}
