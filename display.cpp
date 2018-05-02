#include "display.h"

#include "window.h" // apagar

std::list<Shape*> Display::shapes;
std::list<Shape3D*> Display::shapes3D;

void Display::create_all () {

    Vector3 v0 (0, 0, 0);
    Vector3 v1 (-1, 1, 0);
    Vector3 v2 (1, 1, 0);
    Vector3 v3 (0, 0.5, 1);

    Polyhedron* tetrahedron = new Polyhedron("Illuminati", {
        Edge3D(v0, v1),
        Edge3D(v1, v2),
        Edge3D(v2, v0),
        Edge3D(v3, v0),
        Edge3D(v3, v1),
        Edge3D(v3, v2)
    });

    Window::test = tetrahedron;
    
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

    /*
    std::vector<Shape3D*> import = DescOBJ::read_obj("Figuras.obj");
    for (auto i : import)  {
        shapes3D.push_back(i);
    }
    */

    std::vector<Shape3D*> import = DescOBJ::read_obj("cristo.obj");
    Polyhedron* tpose = dynamic_cast<Polyhedron*>(import.front());
    Window::test = tpose;

    shapes.push_back(curve);
    shapes.push_back(bezier);
    shapes.push_back(spline);
    shapes.push_back(b);
    shapes3D.push_back(tpose);
    //shapes3D.push_back(tetrahedron);
}

void Display::draw_all() {
    
    /*
    for (auto it : shapes) {
        it->draw();
    }
    */

    for (auto it : shapes3D) {
        it->draw();
    }
}

void Display::add(Shape* sh) {
    shapes.push_back(sh);
}

void Display::add(Shape3D* sh) {
    shapes3D.push_back(sh);
}

Shape* Display::find_shape(gchar* name) {
    for (auto it = shapes.begin(); it != shapes.end(); ++it) {
        if ((*it)->name == name) return *it;
    }
    return nullptr;
}

Shape3D* Display::find_shape3D(gchar* name) {
    for (auto it = shapes3D.begin(); it != shapes3D.end(); ++it) {
        if ((*it)->name == name) return *it;
    }
    return nullptr;
}