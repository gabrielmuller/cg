#ifndef POINT_H
#define POINT_H

#include <list>
#include <vector>
#include "vector2.h"
#include "shape.h"

struct Point : Shape {
    Vector2 position;

    Point (std::string name, float x, float y); 
    void draw () override;

    void transform (const Transformation& t) override;
    void translate (const Vector2& distance);
    void scale (const Vector2& amount);
    void rotate (float rad);
    Vector2 center () const override;
};

#endif
