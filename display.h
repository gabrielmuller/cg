#ifndef DISPLAY_H
#define DISPLAY_h

#include <list>
#include "shape.h"
#include "shape3d.h"

// display file
namespace Display {
        extern std::list<Shape*> shapes;
        extern std::list<Shape3D*> shapes3D;
        void create_all ();
        void add(Shape* sh);
    void draw_all ();
}

#endif
