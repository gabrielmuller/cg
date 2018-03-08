#ifndef DISPLAY_H
#define DISPLAY_h

#include "shape.h"
#include <list>

// display file
namespace Display {
    extern std::list<Shape*> shapes;
    void create_all ();
    void add(Shape* sh);
}

#endif
