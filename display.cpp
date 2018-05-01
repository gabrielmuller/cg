#include "display.h"

#include "window.h" // apagar

std::list<Shape*> Display::shapes;
std::list<Shape3D*> Display::shapes3D;

void Display::create_all () {

    std::vector<Shape*> import = DescOBJ::read_obj("Figuras.obj");

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

    Vector3 c0 (0, 0, 0);
    Vector3 c1 (2, 0, 0);
    Vector3 c2 (0, 2, 0);
    Vector3 c3 (2, 2, 0);
    Vector3 c4 (0, 0, 2);
    Vector3 c5 (2, 0, 2);
    Vector3 c6 (0, 2, 2);
    Vector3 c7 (2, 2, 2);

    Polyhedron* cube = new Polyhedron("Cubo", {
        Edge3D(c0, c1),
        Edge3D(c0, c2),
        Edge3D(c0, c4),
        Edge3D(c1, c3),
        Edge3D(c1, c5),
        Edge3D(c2, c3),
        Edge3D(c2, c6),
        Edge3D(c3, c7),
        Edge3D(c4, c5),
        Edge3D(c4, c6),
        Edge3D(c5, c7),
        Edge3D(c6, c7)
    });

    Window::test = cube;
    
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

    for (auto i : import)  {
        shapes.push_back(i);
    }

    shapes.push_back(curve);
    shapes.push_back(bezier);
    shapes.push_back(spline);
    shapes.push_back(b);
    shapes3D.push_back(tetrahedron);
    shapes3D.push_back(cube);
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