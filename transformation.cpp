#include "vector2.h"
#include "vector3.h"
#include <stdexcept>
#include "math.h"
#include "rotation.h"

/**
 *  Matrizes para Bezier
 */
const Transformation Transformation::mb ({
    {-1,  3, -3,  1},
    { 3, -6,  3,  0},
    {-3,  3,  0,  0},
    { 1,  0,  0,  0}
});

/**
 *  Matrizes para B-Spline
 */
const Transformation Transformation::mbs ({
    {(float)-1/6,  (float)1/2, (float)-1/2,  (float)1/6},
    { (float)1/2,          -1,  (float)1/2,           0},
    {(float)-1/2,           0,  (float)1/2,           0},
    { (float)1/6,  (float)2/3,  (float)1/6,           0}
});

/**
 *  Construtor
 */
Transformation::Transformation (int m, int n) : m(m), n(n) {
    matrix = std::vector<std::vector<float>> (m);
    for (int i = 0; i < m; i++) {
        matrix[i] = std::vector<float> (n);
    }
}

/**
 *  Construtor
 */
Transformation::Transformation (std::vector<std::vector<float>> matrix) :
    Transformation(matrix.size(), matrix.front().size()) {

    this->matrix = matrix;
}

void Transformation::transform (Transformation& t) const {
    t = t * *this;
}

Transformation Transformation::transpose () const {
    Transformation t(n, m);
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            t.matrix[j][i] = matrix[i][j];
        }
    }
    return t;
}

/**
 *  Faz a última coluna ser feita de 1's
 *  compensando as outras.
 */
Transformation Transformation::homogenize () {
    for (int i = 0; i < m; i++) {
        float w = matrix[i][n-1];
        if (!w) {
            throw std::domain_error("w é 0!");
        }
        for (int j = 0; j < n; j++) {
            matrix[i][j] /= w;
        }
    }
    return *this;
}

    

/**
 *  Transformações 2D
 */
Transformation Transformation::translation (const Vector2 distance) {
    Transformation t = Transformation (3, 3);
    t.matrix = {{1,0,0},{0,1,0},{distance.x(), distance.y(), 1}};
    return t;
}

Transformation Transformation::scaling (const Vector2 amount, const Vector2 center) {
    Transformation t (3, 3);
    // deslocamento pro centro + escalonamento pré-calculado
    t.matrix = {
        {amount.x(), 0, 0},
        {0, amount.y(), 0},
        {center.x() * (-amount.x() + 1), center.y() * (-amount.y() + 1), 1} 
    };
    return t;
}

Transformation Transformation::rotation (const float rad, const Vector2 center) {
    Transformation t (3, 3);
    //pré calcular trigonometria
    float c = cos(rad);
    float s = sin(rad);

    //matriz pré calculada
    t.matrix = {
        {c, -s, 0},
        {s,  c, 0},
        {-center.y()*s + center.x()*(1-c), center.x()*s + center.y()*(1-c), 1}
    };
    return t;
}

/**
 *  Transformações 3D
 */
Transformation Transformation::translation3D (const Vector3 distance) {
    Transformation t = Transformation (4, 4);
    t.matrix = {{1,0,0,0},{0,1,0,0},{0,0,1,0},{distance.x(), distance.y(), distance.z(), 1}};
    return t;
}

Transformation Transformation::scaling3D (const Vector3 amount, const Vector3 center) {
    Transformation t (4, 4);
    // deslocamento pro centro + escalonamento pré-calculado
    t.matrix = {
        {amount.x(), 0, 0, 0},
        {0, amount.y(), 0, 0},
        {0, 0, amount.z(), 0},
        {center.x() * (-amount.x() + 1), center.y() * (-amount.y() + 1), center.z() * (-amount.z() + 1), 1} 
    };
    return t;
}

Transformation Transformation::rotation3D (const Rotation& rot) {
    const float x = rot.axis.x();
    const float y = rot.axis.y();
    const float z = rot.axis.z();
    const float i = rot.center.x();
    const float j = rot.center.y();
    const float k = rot.center.z();
    const float s = sin(rot.angle);
    const float c = cos(rot.angle);
    const float d = 1 - c;

    Transformation t (4, 4);

    // alias
    auto& v = t.matrix;
    v = {
        {c+x*x*d, x*y*d-z*s, x*z*d+y*s, 0},
        {y*x*d+z*s, c+y*y*d, y*z*d-x*s, 0},
        {z*x*d-y*s, z*y*d+x*s, c+z*z*d, 0},
        {}
    };
    v[3] = {
        -i*v[0][0]-j*v[1][0]-k*v[2][0]+i,
        -i*v[0][1]-j*v[1][1]-k*v[2][1]+j,
        -i*v[0][2]-j*v[1][2]-k*v[2][2]+k,
        1};
    return t;
}

/**
 *  Operador std::string
 */
Transformation::operator std::string () const {
    std::string result;
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            result += std::to_string(matrix[i][j]) + " ";
        }
        result += "\n";
    }
    return result;
}

/**
 *  Operador *
 */
Transformation Transformation::operator* (const Transformation& t) const {
    if (n != t.m) {
        throw std::domain_error("Tamanhos inválidos de matriz");
    }
    Transformation result (m, t.n);

    for (int i = 0; i < m; i++) {
        for (int j = 0; j < t.n; j++) {
            result.matrix[i][j] = 0;
            for (int k = 0; k < n; k++) {
                result.matrix[i][j] += matrix[i][k] * t.matrix[k][j];
            }
        }
    }

    return result;
}
