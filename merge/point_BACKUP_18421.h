#ifndef POINT_H
#define POINT_H

#include <list>
#include <vector>
#include "vector2.h"
#include "shape.h"

struct Point : Shape {
<<<<<<< HEAD
    Point (std::string name, Vector2 position); 
    void draw (cairo_t* ct) override;

    void transform (const Transformation& t) override;
    void translate (const Vector2& distance);
    void scale (const Vector2& amount);
    void rotate (float rad);
    Vector2 center () const override;
=======
    Point (std::string name, std::list<Vector2> verts); 
    void draw (cairo_t* cr);
    
    void translation (float dx, float dy);
    void scaling(float sx, float sy);
    void rotation(float dx, float dy, float degrees, bool center);
>>>>>>> notebook
};

#endif
