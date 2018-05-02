#ifndef DISPLAY_H
#define DISPLAY_h

#include <list>
#include <math.h>

#include "bezier.h"
#include "descobj.h"
#include "line.h"
#include "point.h"
#include "point3d.h"
#include "polyhedron.h"
#include "shape.h"
#include "shape3d.h"
#include "spline.h"

// display file
namespace Display {
    extern std::list<Shape*> shapes;
    extern std::list<Shape3D*> shapes3D;
    void create_all ();
    void add(Shape* sh);
    void add(Shape3D* sh);
    void draw_all ();
    Shape* find_shape(gchar* name);
    Shape3D* find_shape3D(gchar* name);
}

#endif
