#ifndef DISPLAY_H
#define DISPLAY_h

#include <list>
#include <math.h>

#include "shape.h"
#include "point.h"
#include "line.h"
#include "spline.h"
#include "bezier.h"
#include "shape3d.h"
#include "polyhedron.h"
#include "descobj.h"

// display file
namespace Display {
    extern std::list<Shape*> shapes;
    extern std::list<Shape3D*> shapes3D;
    void create_all ();
    void add(Shape* sh);
    void add(Shape3D* sh);
    void draw_all ();
}

#endif
