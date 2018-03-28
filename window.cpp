#include "window.h"
#include "math.h"

Vector2 Window::position(0, 0);
Vector2 Window::viewport(400, 400);
float Window::angle = 0;

// tamanho da visão da câmera (mundo)
Vector2 Window::size(10, 10);

// converte uma coordenada do espaço no mundo para tela
Vector2 Window::world_to_screen(Vector2 coords) {
    return norm_to_vp(world_to_norm(coords));
}

// converte uma coordenada do espaço no mundo para
// coordenada normalizada
Vector2 Window::world_to_norm (Vector2 coords) {
    float s = sin(angle);
    float c = cos(angle);
    float a = 2 / size.x();
    float b = 2 / size.y();
    float x = position.x();
    float y = position.y();

    Transformation t (3, 3);
    t.matrix = {
        {a*c, -b*s, 0},
        {a*s,  b*c, 0},
        {a*(-c*x - s*y), b*(s*x - c*y), 1}
    };

    coords = (Transformation) coords * t;
    return coords;
}

Vector2 Window::norm_to_vp (Vector2 coords) {
    Vector2 amount (viewport.x() / 2, -viewport.y() / 2);
    float a = viewport.x() / 2;
    float b = -viewport.y() / 2;

    Transformation t (3, 3);
    t.matrix = {
        {a,  0, 0},
        {0,  b, 0},
        {a, -b, 1}
    };

    coords = (Transformation) coords * t;
    return coords;
}
