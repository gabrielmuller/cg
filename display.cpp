#include "display.h"

#include "spline.h"
#include "bezier.h"
#include "point.h"
#include "line.h"
#include "polyhedron.h"
#include <math.h>
#include "descobj.h"

std::list<Drawable*> Display::dbs;

void Display::create_all () {

    std::vector<Drawable*> import = DescOBJ::read_obj("Figuras.obj");

    Vector3 v0 (0, 0, 0);
    Vector3 v1 (-1, 1, 0);
    Vector3 v2 (1, 1, 0);
    Vector3 v3 (0, 0.5, 1);

    Polyhedron* cube = new Polyhedron("Cubo", {
        Edge3D(v0, v1),
        Edge3D(v1, v2),
        Edge3D(v2, v0),
        Edge3D(v0, v1),
        Edge3D(v0, v2),
        Edge3D(v0, v3)
    });
    
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
    
    for (auto i : import)  {
        dbs.push_back(i);
    }

    dbs.push_back(curve);
    dbs.push_back(bezier);
    dbs.push_back(spline);
    dbs.push_back(b);
    dbs.push_back(cube);
}

void Display::draw_all() {
    for (auto it = dbs.begin();
        it != dbs.end();
        ++it) {
        (*it)->draw();
    }
}

void Display::add(Drawable* sh) {
    dbs.push_back(sh);
}
