#ifndef SHAPE_H
#define SHAPE_H

#include <string>
#include <gtk/gtk.h>
#include "vector2.h"
#include "window.h"

struct Shape {
    std::string name;
    Vector2 position;

    Shape (std::string name, Vector2 position); 
    virtual void draw (cairo_t* ct) = 0;

    virtual void transform (const Transformation& t) = 0;
    void translate (const Vector2& distance);
    void scale (const Vector2& amount);
    void rotate (float rad);
    virtual Vector2 center () const = 0;
};

#endif
