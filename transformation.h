#ifndef TRANSFORMATION_H
#define TRANSFORMATION_H
#include <vector>
#include <string>

struct Vector2;

struct Transformation {
    std::vector<std::vector<float>> matrix;
    int m; //linhas
    int n; //colunas
    static const Transformation mb;
    static const Transformation mbs;

    Transformation (int m, int n);
    Transformation (std::vector<std::vector<float>> matrix);
    Transformation operator* (const Transformation& t) const;

    static Transformation translation (const Vector2 distance);
    static Transformation scaling (const Vector2 amount, const Vector2 center);
    static Transformation rotation (const float rad, const Vector2 center);

    void transform (Transformation& t) const;
    operator std::string () const;
    //Transformation& operator= (const Transformation& other);
};

#endif
