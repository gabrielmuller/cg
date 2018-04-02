#include "window.h"
#include "math.h"

Vector2 Window::viewport(400, 400);
float Window::smooth = 0.2;
float Window::xl, Window::xr, Window::yd, Window::yu;
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

AB Window::clip_line (AB line) {
    int a_rc = get_rc(line.a);
    int b_rc = get_rc(line.b);
    
    // se os dois RC são 0000, não ocorre clipping
    if (!a_rc && !b_rc) {
        return line;
    }

    // se o AND dos dois RC é diferente de 0000, está fora da janela
    // retorna empty = true
    if (a_rc & b_rc) {
        return AB();
    }

    // clipping de fato

    // outside é o ponto que está fora da window e será alterado
    Vector2& out_point = a_rc ? line.a : line.b;
    int& out_rc = a_rc ? a_rc : b_rc;
    // por enquanto só pela esquerda
    if (out_rc & (1 << 0)) {
        float m = (line.a.x() - line.b.x()) / (line.a.y() - line.b.y());
        out_point = Vector2(xl, m*(xl - out_point.x()) + out_point.y());
    }

    return line;
}

/* Exemplo de RC:
 * 0000 0110
 *      NSLO (pontos cardeais)
 * valor 6 (sudeste)
 */
int Window::get_rc (Vector2 point) {
    // TODO: chamar update_boundaries num local melhor
    update_boundaries();
    float x = point.x();
    float y = point.y();
    int rc = 0;

    rc |= (x < xl) << 0;
    rc |= (x > xr) << 1;
    rc |= (y < yd) << 2;
    rc |= (y > yu) << 3;

    return rc;
}

void Window::update_boundaries () {
    // margem para ver se clipping realmente funciona
    Vector2 mock_size = real.size - Vector2(1, 1);

    xl = real.position.x() - mock_size.x()/2;
    xr = real.position.x() + mock_size.x()/2;
    yd = real.position.y() - mock_size.y()/2;
    yu = real.position.y() + mock_size.y()/2;
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
