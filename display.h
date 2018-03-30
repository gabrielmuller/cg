#ifndef DISPLAY_H
#define DISPLAY_h

#include <list>
#include "shape.h"
#include <gtk/gtk.h>

// display file
namespace Display {
    extern std::list<Shape*> shapes;
    void create_all ();
    void add(Shape* sh);
    void draw_all (cairo_t* cr);
}

#endif
