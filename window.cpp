#include "window.h"
#include "math.h"

Vector2 Window::viewport(400, 400);
float Window::smooth = 0.2;

// converte uma coordenada do espaço no mundo para tela
Vector2 Window::world_to_screen(Vector2 coords) {
    return norm_to_vp(world_to_norm(coords));
}

Specs::Specs () : position(Vector2(0, 0)), size(Vector2(10, 10)), angle(0) {}

Specs Window::real;
Specs Window::goal;

// converte uma coordenada do espaço no mundo para
// coordenada normalizada
Vector2 Window::world_to_norm (Vector2 coords) {
    float s = sin(real.angle);
    float c = cos(real.angle);
    float a = 2 / real.size.x();
    float b = 2 / real.size.y();
    float x = real.position.x();
    float y = real.position.y();

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

void Window::animate () {
    real.position = Vector2::lerp(real.position, goal.position, smooth);
    real.size = Vector2::lerp(real.size, goal.size, smooth);

    // lerp ângulo
    float a = fmodf(real.angle, (float) M_PI * 2);
    float b = fmodf(goal.angle, (float) M_PI * 2);
    float& largest = a > b ? a : b;
    float& smallest = a < b ? a : b;
    if (largest - smallest > M_PI) {
        largest -= M_PI * 2;
    }
    real.angle = a * (1-smooth) + b * smooth;
}
