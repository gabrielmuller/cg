#include "window.h"
#include "math.h"

Vector2 Window::position(0, 0);
Vector2 Window::viewport(400, 400);
float Window::angle = 0;

// tamanho da visão da câmera (mundo)
Vector2 Window::size(10, 10);

// converte uma coordenada do espaço no mundo para tela
Vector2 Window::world_to_screen(Vector2 coords) {
    /*Transformation* rot = Transformation::rotation(angle, Window::position);
    coords = *((Transformation) coords * *rot);

    Vector2 wmin = Window::position - (Window::size / 2);
    Vector2 output(coords - wmin);
    output = Vector2(output.x() / Window::size.x(), output.y() / Window::size.y());
    output.matrix[0][1] = 1 - output.y();
    output = Vector2(output.x() * Window::viewport.x(), output.y() * Window::viewport.y());

    return output;*/
    return norm_to_vp(world_to_norm(coords));
}


// TODO: otimizar

// converte uma coordenada do espaço no mundo para
// coordenada normalizada
Vector2 Window::world_to_norm (Vector2 coords) {
    Transformation t (3, 3);
    float s = sin(angle);
    float c = cos(angle);
    float a = 2 / size.x();
    float b = 2 / size.y();
    float x = position.x();
    float y = position.y();

    t.matrix = {
        {a*c, -b*s, 0},
        {a*s,  b*c, 0},
        {a*(-c*x - s*y), b*(s*x - c*y), 1}
    };

    coords = *((Transformation) coords * t);
    return coords;
}

Vector2 Window::norm_to_vp (Vector2 coords) {
    Vector2 amount (viewport.x() / 2, -viewport.y() / 2);
    float a = viewport.x() / 2;
    float b = -viewport.y() / 2;
    Transformation* move = Transformation::translation(Vector2(1, -1));
    Transformation* scale = Transformation::scaling(amount, Vector2(0, 0));
    Transformation t (3, 3);
    t.matrix = {
        {a,  0, 0},
        {0,  b, 0},
        {a, -b, 1}
    };

    coords = *((Transformation) coords * t);
    return coords;
}
