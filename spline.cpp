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

    // quanto maior, mais precisão
    float sigma = 0.02;
    int samples = (float)1/sigma;

    // aumenta precisão com zoom
    samples /= Window::real.size.x();

    // garantir que tamanho mod 3 é 1
    int remainder = verts.size() % 3;

    if (remainder != 1) {
        // 0 1 2 3
        // 3 4 5 _
        // faz uma quadrática equivalente
        // duplica penúltimo vértice
        auto vert = verts.end() - 2;
        verts.insert(vert, *vert);
    }
    if (remainder == 2) {
        // 0 1 2 3
        // 3 4 _ _
        // faz uma reta equivalente
        // duplica últimos dois vértices
        // (penúltimo já foi duplicado no primeiro if)
        verts.push_back(verts.back());
    }

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
    for (int i = 0; i < size - 4; i++) {
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

    float xo, yo;
    xo = x;
    yo = y;
    Vector2 a = Vector2(xo, yo);
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