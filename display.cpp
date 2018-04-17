#include "display.h"

#include "spline.h"
#include "bezier.h"
#include "point.h"
#include "line.h"
#include <math.h>
#include "descobj.h"

std::list<Shape*> Display::shapes;

void Display::create_all () {

    std::vector<Shape*> import = DescOBJ::read_obj("Figuras.obj");
    
    Bezier* curve = new Bezier ("Curva", {
        Vector2(-5, -2),
        Vector2(-3, -3),
        Vector2( 1, -1.5),
        Vector2( 2, -5),
        Vector2( 3, -10),
        Vector2( 5, -1.7),
        Vector2(-3, -7),
        Vector2( 2, -6),
        Vector2( 2, -11)
    });

    Bezier* bezier = new Bezier ("Bezier", {
        Vector2(1,1),
        Vector2(2,3),
        Vector2(3,0),
        Vector2(4,1),
        Vector2(5,2),
        Vector2(4,4),
        Vector2(6,4),
        Vector2(7,4),
        Vector2(6,2),
        Vector2(7,1)
    });

    Spline* b = new Spline ("Bbbb", {
        Vector2(0, 0),
        Vector2(0, 1),
        Vector2(1, 1),
        Vector2(1, 0)
    });

    Spline* spline = new Spline ("B-Spline", {
        Vector2(1,1),
        Vector2(2,2),
        Vector2(3,2),
        Vector2(4,1),
        Vector2(5,2),
        Vector2(4,4),
        Vector2(6,4),
        Vector2(7,4),
        Vector2(6,2),
        Vector2(7,1)
    });

    //bezier->translate(Vector2(2,2));
    //spline->translate(Vector2(2,0));
    
    for (auto i : import) 
        shapes.push_back(i);
    shapes.push_back(curve);
    shapes.push_back(bezier);
    shapes.push_back(spline);
    shapes.push_back(b);
}

void Display::draw_all() {
    for (auto it = shapes.begin();
        it != shapes.end();
        ++it) {
        (*it)->draw();
    }
}

void Display::add(Shape* sh) {
    shapes.push_back(sh);
}
