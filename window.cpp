#include "window.h"
#include "math.h"
#include <iostream>


float t;
Vector2 Window::viewport(400, 400);
float Window::smooth = 0.2;
float Window::xl, Window::xr, Window::yd, Window::yu;
float Window::clip_margin = 1.8;

/**
 *  Bordas da Viewport. {esquerda, cima, direita, baixo}
 */
std::list<Edge> Window::edges() {
    return {
            Edge(Vector2(xl, yd), Vector2(xl,yu)),
            Edge(Vector2(xl, yu), Vector2(xr,yu)),
            Edge(Vector2(xr, yu), Vector2(xr,yd)),
            Edge(Vector2(xr, yd), Vector2(xl,yd))
        };
}

int Window::clipping_algorithm = COHEN_SUTHERLAND;

cairo_t* Window::cr;

Specs::Specs () : position(Vector2(0, 0)), size(Vector2(10, 10)), angle(0) {}

Specs3D::Specs3D () : position(Vector3(0, 0, -10)), fov(75),
    forward(Vector3(0, 0, 1)) {}

Specs Window::real;
Specs Window::goal;
Specs3D Window::real3;

/*void Window::rotate(float angle) {
    
}*/

const Transformation Window::projection_matrix() {

    // Translação
    const Vector3 vrp (-real3.position.x(), -real3.position.y(), -real3.position.z());
    Transformation trans = Transformation::translation3D(vrp);

    const Vector3 normal = real3.forward;

    // Rotação x e y
    float tetax = std::atan(normal.y() / normal.z());
    float tetay = std::atan(normal.x() / normal.z());

    Transformation rotx(4,4);
    Transformation roty(4,4);
    Transformation scale(4,4);
    rotx.matrix = {
        { 1, 0, 0, 0 },
        { 0,  std::cos(tetax), std::sin(tetax), 0 },
        { 0, -std::sin(tetax), std::cos(tetax), 0 },
        { 0, 0, 0, 1 }
    };

    roty.matrix = {
        { std::cos(tetay), 0, -std::sin(tetay), 0 },
        { 0, 1, 0, 0 },
        { std::sin(tetay), 0, std::cos(tetay), 0 },
        { 0, 0, 0, 1 }
    };

    return trans * rotx * roty;
}

/*****************************************
 *
 *  Funções de transformação de coordenada
 *
 *****************************************/

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

Vector2 Window::world_to_norm (Vector3 coords) {
    coords = coords * projection_matrix();
    return world_to_norm((Vector2)coords);
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

/*****************************************
 *
 *  Funções de Draw
 *
 *****************************************/

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

void Window::draw_point (Vector3 point) {
    draw_point(Vector2(point.x(), point.y()));
}

// Recebe uma linha em coordenadas de mundo,
// transforma em coordenadas normalizadas,
// faz clipping e desenha no viewport
void Window::draw_line (Edge line) {
    line.a = world_to_norm(line.a);
    line.b = world_to_norm(line.b);

    try {
        line = clip_line(line);
    }
    catch (std::exception e) {
        // se não precisa renderizar retorna
        return;
    }

    line.a = norm_to_vp(line.a);
    line.b = norm_to_vp(line.b);

    cairo_move_to(cr, line.a.x(), line.a.y());
    cairo_line_to(cr, line.b.x(), line.b.y());
    cairo_stroke(cr);
    
}

void Window::draw_line (Edge3D line) {
    Edge norm (world_to_norm(line.a), world_to_norm(line.b));
    // fazer clipping 3D depois
    try {
        norm = clip_line(norm);
    }
    catch (std::exception e) {
        // se não precisa renderizar retorna
        return;
    }

    norm.a = norm_to_vp(norm.a);
    norm.b = norm_to_vp(norm.b);

    cairo_move_to(cr, norm.a.x(), norm.a.y());
    cairo_line_to(cr, norm.b.x(), norm.b.y());
    cairo_stroke(cr);
}

// Parecido com draw_line mas sem clipping
// Usada para polígonos preenchidos
void Window::draw_pline (Edge line) {
    line.a = norm_to_vp(line.a);
    line.b = norm_to_vp(line.b);
    cairo_line_to(cr, line.b.x(), line.b.y());
    //cairo_stroke_preserve(cr);
}

/*****************************************
 *
 *  Funções de Clipping
 *
 *****************************************/

Edge Window::clip_line (Edge line) {
    if (clipping_algorithm == COHEN_SUTHERLAND) {
        return clip_cs(line);
    } else if (clipping_algorithm == LIANG_BARSKY) {
        return clip_lb(line);
    }

    throw std::exception();
}

// Cohen-Sutherland
Edge Window::clip_cs (Edge line) {
    int a_rc = get_rc(line.a);
    int b_rc = get_rc(line.b);
    
    // se os dois RC são 0000, não ocorre clipping
    if (!a_rc && !b_rc) {
        return line;
    }

    // se o AND dos dois RC é diferente de 0000, está fora da janela
    // retorna exceção
    if (a_rc & b_rc) {
        throw std::exception();
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
            if (y <= yu && y >= yd) {
                *p[i] = Vector2(xl, y);
                changed = true;
            }
        }
        if (rc[i] & (1 << XR)) {
            float y = m*(xr - p[i]->x()) + p[i]->y();
            if (y <= yu && y >= yd) {
                *p[i] = Vector2(xr, y);
                changed = true;
            }
        }
        if (rc[i] & (1 << YD)) {
            float x = (yd - p[i]->y())/m + p[i]->x();
            if (x <= xr && x >= xl) {
                *p[i] = Vector2(x, yd);
                changed = true;
            }
        }
        if (rc[i] & (1 << YU)) {
            float x = (yu - p[i]->y())/m + p[i]->x();
            if (x <= xr && x >= xl) {
                *p[i] = Vector2(x, yu);
                changed = true;
            }
        }
    }

    // se não houve, a reta está completamente fora
    if (!changed) {
        throw std::exception();
    }
    return line;
}

// Liang-Barsky
Edge Window::clip_lb (Edge line) {
    float delta_x = line.b.x() - line.a.x();
    float delta_y = line.b.y() - line.a.y();
    float p[4] = {-delta_x, delta_x, -delta_y, delta_y};
    float q[4] = {line.a.x() - xl, xr - line.a.x(), line.a.y() - yd, yu - line.a.y()};

    for (int i = 0; i < 4; i ++) {
        if (p[i] == 0 && q[i] < 0)  {
            // linha completamente fora
            throw std::exception();
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
        throw std::exception();
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

// Clipa linha pra uma borda, usado no clipping de polígonos
Vector2 Window::clip_to_edge(Edge edge, Edge line) {
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

// Checa se um ponto está dentro de uma borda da window
bool Window::is_inside(Vector2 coord, Edge edge) {
    return (edge.b.x() - edge.a.x()) * (coord.y() - edge.a.y()) < 
           (edge.b.y() - edge.a.y()) * (coord.x() - edge.a.x());
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

/*****************************************
 *
 *  Margem para clipping
 *
 *****************************************/

void Window::draw_borders () {
    cairo_set_source_rgb(cr, 1, 0, 0);
    Vector2 a = norm_to_vp(Vector2(xl, yu));
    Vector2 b = norm_to_vp(Vector2(xr, yu));
    Vector2 c = norm_to_vp(Vector2(xr, yd));
    Vector2 d = norm_to_vp(Vector2(xl, yd));
    cairo_move_to(cr, a.x(), a.y());
    cairo_line_to(cr, b.x(), b.y());
    cairo_line_to(cr, c.x(), c.y());
    cairo_line_to(cr, d.x(), d.y());
    cairo_line_to(cr, a.x(), a.y());
    cairo_stroke(cr);
}

void Window::update_boundaries () {
    // margem para ver se clipping realmente funciona
    // em coordenadas normalizadas
    Vector2 mock_size = Vector2(clip_margin, clip_margin);

    xl = -mock_size.x()/2;
    xr =  mock_size.x()/2;
    yd = -mock_size.y()/2;
    yu =  mock_size.y()/2;
}

/*****************************************
 *
 *  Função de animação
 *
 *****************************************/

void Window::animate () {
    update_boundaries();
    real.position = Vector2::lerp(real.position, goal.position, smooth);
    //real.size = Vector2::lerp(real.size, goal.size, smooth);

    t += 0.016666667;
    float bpm = 123;
    float size = (pow(std::sin(t * bpm * 2 / 60), 4) + 1 ) * 5;
    real3.position = Vector3(std::cos(t), std::sin(t), std::cos(t)+1.1);
    real3.forward = Vector3(-real3.position.x(), -real3.position.y(), -real3.position.z());
    real.size = Vector2(size, size);
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
