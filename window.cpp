#include "window.h"
#include "math.h"
#include <iostream>

Vector2 Window::viewport(400, 400);
float Window::smooth = 0.2;
float Window::xl, Window::xr, Window::yd, Window::yu;

/**
 *  Bordas da Viewport. {esquerda, cima, direita, baixo}
 */
std::list<AB> Window::edges() {
    return {
            AB(Vector2(xl, yd), Vector2(xl,yu)),
            AB(Vector2(xl, yu), Vector2(xr,yu)),
            AB(Vector2(xr, yu), Vector2(xr,yd)),
            AB(Vector2(xr, yd), Vector2(xl,yd))
        };
}

int Window::clipping_algorithm = COHEN_SUTHERLAND;

cairo_t* Window::cr;
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

// Recebe uma linha em coordenadas de mundo,
// transforma em coordenadas normalizadas,
// faz clipping e desenha no viewport
void Window::draw_line (AB line) {
    line.a = world_to_norm(line.a);
    line.b = world_to_norm(line.b);

    line = clip_line(line);

    line.a = norm_to_vp(line.a);
    line.b = norm_to_vp(line.b);

    cairo_move_to(cr, line.a.x(), line.a.y());
    cairo_line_to(cr, line.b.x(), line.b.y());
    cairo_stroke(cr);
    
}

// Parecido com draw_line mas sem clipping
// Usada para polígonos preenchidos
void Window::draw_pline (AB line) {
    line.a = norm_to_vp(line.a);
    line.b = norm_to_vp(line.b);
    cairo_line_to(cr, line.b.x(), line.b.y());
    //cairo_stroke_preserve(cr);
}

// Checa se um ponto está dentro de uma borda da window
bool Window::is_inside(Vector2 coord, AB edge) {
    return (edge.b.x() - edge.a.x()) * (coord.y() - edge.a.y()) < 
           (edge.b.y() - edge.a.y()) * (coord.x() - edge.a.x());
}

// Clipa linha pra uma borda, usado no clipping de polígonos
Vector2 Window::clip_to_edge(AB edge, AB line) {
    Vector2 v = (!is_inside(line.a,edge)) ? line.a : line.b;
    float m = (line.a.y() - line.b.y()) / (line.a.x() - line.b.x());
    if (edge.a.x() == edge.b.x()) //direita/esquerda
    {   
        float y = (edge.a.x()-v.x())*m + v.y();
        v = Vector2(edge.a.x(), y);
    } else //cima/baixo
    {   
        float x = 0;
        if (m == 0) x = v.x();
        else x = (edge.a.y()-v.y())/m + v.x();
        v = Vector2(x, edge.a.y());
    } 
    return v;
}

void Window::draw_point (Vector2 point) {
    point = world_to_norm(point);

    // C L I P P
    if ((point.x() < xl || point.x() > xr || 
        point.y() < yd || point.y() > yu)) {
        return;
    }

    point = norm_to_vp(point);
    cairo_move_to(cr, point.x() - 1, point.y());
    cairo_line_to(cr, point.x() + 1, point.y());
    cairo_stroke(cr);
}

AB Window::clip_line (AB line) {
    if (clipping_algorithm == COHEN_SUTHERLAND) {
        return clip_cs(line);
    } else if (clipping_algorithm == LIANG_BARSKY) {
        return clip_lb(line);
    }
}

AB Window::clip_cs (AB line) {
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

    // coeficiente angular
    float m = (line.a.y() - line.b.y()) / (line.a.x() - line.b.x());

    // colocar em arrays para poder fazer loop
    Vector2* p[2] = {&line.a, &line.b};
    int rc[2] = {a_rc, b_rc};

    // houve mudança em um dos pontos?
    bool changed = false;

    for (int i = 0; i < 2; i++) {
        if (!rc[i]) {
            continue;
        }

        // TODO código repetido, dá pra parametrizar
        if (rc[i] & (1 << XL)) {
            float y = m*(xl - p[i]->x()) + p[i]->y();
            if (y < yu && y > yd) {
                *p[i] = Vector2(xl, y);
                changed = true;
            }
        }
        if (rc[i] & (1 << XR)) {
            float y = m*(xr - p[i]->x()) + p[i]->y();
            if (y < yu && y > yd) {
                *p[i] = Vector2(xr, y);
                changed = true;
            }
        }
        if (rc[i] & (1 << YD)) {
            float x = (yd - p[i]->y())/m + p[i]->x();
            if (x < xr && x > xl) {
                *p[i] = Vector2(x, yd);
                changed = true;
            }
        }
        if (rc[i] & (1 << YU)) {
            float x = (yu - p[i]->y())/m + p[i]->x();
            if (x < xr && x > xl) {
                *p[i] = Vector2(x, yu);
                changed = true;
            }
        }
    }

    // se não houve, a reta está completamente fora
    if (!changed) {
        line = AB();
    }
    return line;
}


AB Window::clip_lb (AB line) {
    float delta_x = line.b.x() - line.a.x();
    float delta_y = line.b.y() - line.a.y();
    float p[4] = {-delta_x, delta_x, -delta_y, delta_y};
    float q[4] = {line.a.x() - xl, xr - line.a.x(), line.a.y() - yd, yu - line.a.y()};

    for (int i = 0; i < 4; i ++) {
        if (p[i] == 0 && q[i] < 0)  {
            // linha completamente fora
            return AB();
        }
    }
    float u1 = 0;
    float u2 = 1; // with or without you
    for (int i = 0; i < 4; i++) {
        if (p[i] < 0) {
            u1 = std::max(u1, q[i]/p[i]);
        }
        if (p[i] > 0) {
            u2 = std::min(u2, q[i]/p[i]);
        }
    }
    if (u1 > u2) {
        // linha completamente fora
        return AB();
    }
    if (u1 > 0) {
        // de fora pra dentro
        line.a = Vector2(line.a.x() + u1 * delta_x, line.a.y() + u1 * delta_y);
    }
    if (u2 < 1) {
        // de dentro pra fora
        line.b = Vector2(line.a.x() + u2 * delta_x, line.a.y() + u2 * delta_y);
    }

    return line;
}

/* Exemplo de RC:
 * 0000 0110
 *      NSLO (pontos cardeais)
 * valor 6 (sudeste)
 */
int Window::get_rc (Vector2 point) {
    float x = point.x();
    float y = point.y();
    int rc = 0;

    rc |= (x < xl) << XL;
    rc |= (x > xr) << XR;
    rc |= (y < yd) << YD;
    rc |= (y > yu) << YU;

    return rc;
}

void Window::update_boundaries () {
    // margem para ver se clipping realmente funciona
    // em coordenadas normalizadas
    Vector2 mock_size = Vector2(1.8, 1.8);

    xl = -mock_size.x()/2;
    xr =  mock_size.x()/2;
    yd = -mock_size.y()/2;
    yu =  mock_size.y()/2;
}

void Window::animate () {
    update_boundaries();
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
