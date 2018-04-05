#include "bezier.h"
#include "vertices.h"

Bezier::Bezier(std::string name, std::vector<Vector2> verts) 
    : Vertices::Vertices(name, verts) {}

void Bezier::draw () {
    if (verts.empty() || verts.size() == 1) {
        return;
    }

    // quanto maior, mais precisão
    int samples = 500;

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

    int size = verts.size();
    // para cada 4 pontos
    // ex: 0 1 2 3
    //     3 4 5 6 ...
    for (int i = 0; i < size - 1; i += 3) {
        Transformation x (1, 4);
        Transformation y (1, 4);

        // x e y são colunas com os valores dos pontos
        for (int j = 0; j < 4; j++) {
            x.matrix[0][j] = verts[i + j].x();
            y.matrix[0][j] = verts[i + j].y();
        }

        // calcular coeficientes das equações paramétricas
        Transformation xt = x * Transformation::mb;
        Transformation yt = y * Transformation::mb;
        std::vector<float> coeffs_x = xt.matrix[0];
        std::vector<float> coeffs_y  = yt.matrix[0];


        Vector2 a = Vector2(t_times(coeffs_x, 0), t_times(coeffs_y, 0));
        for (float t = 0; t < 1; t += (float) 1 / samples) {
            Vector2 b = Vector2(t_times(coeffs_x, t), t_times(coeffs_y, t));
            Window::draw_line(AB(a, b));
            a = b;
        }
        Vector2 b = Vector2(t_times(coeffs_x, 1), t_times(coeffs_y, 1));
        Window::draw_line(AB(a, b));
    }
}

// multiplica um vetor por T (ex t³, t², t, 1)
float Bezier::t_times(std::vector<float> v, float t) {
    float result = 0;
    for (int i = 0; i < v.size(); i++) {
        result += v[i] * pow(t, v.size() - i - 1);
    }
    return result;
}
