#ifndef TRANSFORMATION_H
#define TRANSFORMATION_H
#include <vector>
#include <string>

struct Vector2;

struct Transformation {
    std::vector<std::vector<float>> matrix;
    int m; //linhas
    int n; //colunas

    Transformation (int m, int n);
    Transformation* operator* (const Transformation& t) const; 
    void transform (Transformation*& t) const;
    operator std::string () const;
};

#endif
