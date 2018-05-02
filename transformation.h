#ifndef TRANSFORMATION_H
#define TRANSFORMATION_H
#include <vector>
#include <string>

struct Vector2;
struct Vector3;
struct Rotation;

struct Transformation {
    std::vector<std::vector<float>> matrix;
    int m; //linhas
    int n; //colunas
    static const Transformation mb;     // Bezier
    static const Transformation mbs;    // B-Spline

    Transformation (int m, int n);
    Transformation (std::vector<std::vector<float>> matrix);
    void transform (Transformation& t) const;
    
    static Transformation translation (const Vector2 distance);
    static Transformation scaling (const Vector2 amount, const Vector2 center);
    static Transformation rotation (const float rad, const Vector2 center);
    
    static Transformation translation3D (const Vector3 distance);
    static Transformation scaling3D (const Vector3 amount, const Vector3 center);
    static Transformation rotation3D (const Rotation& rot);

    static Transformation rotatex(const float rad);
    static Transformation rotatey(const float rad);
    static Transformation rotatez(const float rad);

    Transformation operator* (const Transformation& t) const;
    operator std::string () const;
};

#endif
