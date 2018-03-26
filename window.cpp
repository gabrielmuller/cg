#include "window.h"

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
    Vector2 amount (2 / size.x(), 2 / size.y());
    Transformation* rot = Transformation::rotation(angle, position);
    Transformation* scale = Transformation::scaling(amount, position);
    Transformation* move = Transformation::translation(Vector2(-position.x(), -position.y()));
    //ta horrível
    coords = *(*(*((Transformation) coords * *rot) * *scale) * *move);
    delete rot;
    delete scale;
    return coords;
}

Vector2 Window::norm_to_vp (Vector2 coords) {
    Vector2 amount (viewport.x() / 2, -viewport.y() / 2);
    Transformation* move = Transformation::translation(Vector2(1, -1));
    Transformation* scale = Transformation::scaling(amount, Vector2(0, 0));
    return *(*((Transformation) coords * *move) * *scale);
}

    
