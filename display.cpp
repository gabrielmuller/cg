#include "display.h"
#include <math.h>

std::list<Polygon> Display::polys;

void Display::create_all () {
    Polygon* poly = new Polygon("Poly", Vector2(1, 2));

    poly->verts.push_back(Vector2(0, 0));
    poly->verts.push_back(Vector2(0, 3));
    poly->verts.push_back(Vector2(3, 5));
    poly->verts.push_back(Vector2(0, 0));

    Polygon* square = new Polygon("Square", Vector2(-1, -1));
    square->verts.push_back(Vector2(0, 0));
    square->verts.push_back(Vector2(0, 2));
    square->verts.push_back(Vector2(2, 2));
    square->verts.push_back(Vector2(2, 0));
    square->verts.push_back(Vector2(0, 0));

    Polygon* wave = new Polygon ("Sinewave", Vector2(-5, -5));

    for (int i = 0; i < 20; i++) {
        wave->verts.push_back(Vector2(i, sin(i)));
    }
        

    polys.push_back(*poly);
    polys.push_back(*square);
    polys.push_back(*wave);
}
