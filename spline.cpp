#include "spline.h"
#include "vertices.h"
#include <iostream>

Spline::Spline(std::string name, std::vector<Vector2> verts) 
    : Vertices::Vertices(name, verts) {}

Spline::Spline(std::string name) 
    : Vertices::Vertices(name) {}

void Spline::draw () {
    if (verts.empty() || verts.size() == 1) {
        return;
    }

    float sigma = 0.02;
    int samples = (float)1/sigma;

    // aumenta precisão com zoom
    samples /= Window::real.size.x();

    // Matriz E(sigma)
    Transformation e ({
        {              0,                 0,      0,  1},
        {  powf(sigma, 3),   powf(sigma, 2),  sigma,  0},
        {6*powf(sigma, 3), 2*powf(sigma, 2),      0,  0},
        {6*powf(sigma, 3),                0,      0,  0}
    });

    int size = verts.size();
    // para cada 4 pontos
    // ex: 0 1 2 3
    //     1 2 3 4 ...
    for (int i = 0; i < size - 3; i++) {
        Transformation x (4, 1);
        Transformation y (4, 1);

        // Gx é uma coluna das coordenadas x
        for (int j = 0; j < 4; j++) {
            x.matrix[j][0] = verts[i + j].x();
            y.matrix[j][0] = verts[i + j].y();
        }

        // calcular coeficientes das equações paramétricas
        // Cx = mbs * Gx
        Transformation xt = Transformation::mbs * x;
        Transformation yt = Transformation::mbs * y;

        // E(sigma) * Cx = (x, dfx, d2fx, d3fx)
        Transformation fx_m = e * xt;
        Transformation fy_m = e * yt;
        std::vector<float> fx = {
            fx_m.matrix[0][0],
            fx_m.matrix[1][0],
            fx_m.matrix[2][0],
            fx_m.matrix[3][0]
        };
        std::vector<float> fy = {
            fy_m.matrix[0][0],
            fy_m.matrix[1][0],
            fy_m.matrix[2][0],
            fy_m.matrix[3][0]
        };

        fwdDiff((float)1/sigma, fx[0], fx[1], fx[2], fx[3],
            fy[0], fy[1], fy[2], fy[3]);
    }
}

void Spline::fwdDiff (float n, float x, float dx, float d2x, float d3x, 
    float y, float dy, float d2y, float d3y) {

    Vector2 a = Vector2(x, y);
    for (float i = 0; i < n; i++) {
        x   += dx;
        dx  += d2x;
        d2x += d3x;
        y   += dy;
        dy  += d2y;
        d2y += d3y;
        Vector2 b = Vector2(x, y);
        Window::draw_line(AB(a, b));
        a=b;
    }
}
