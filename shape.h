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

    virtual void translation(float dx, float dy) = 0;
    virtual void scaling(float sx, float sy) = 0;
    virtual void rotation(float dx, float dy, float degrees, bool center) = 0;
};

#endif
