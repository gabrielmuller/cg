#include "window.h"

Vector2 Window::position(0, 0);
Vector2 Window::viewport(400, 400);

// tamanho da visão da cãmera (mundo)
Vector2 Window::size(10, 10);

// converte uma coordenada do espaço no mundo para tela
Vector2 Window::world_to_screen(Vector2 coords) {
    Vector2 wmin = Window::position - (Window::size * Vector2(0.5, 0.5));
    Vector2 output(coords - wmin);
    output = Vector2(output.x() / Window::size.x(), output.y() / Window::size.y());
    output.y() = 1 - output.y();
    output = output * Window::viewport;

    return output;
}
