#include "display.h"
#include <math.h>

std::list<Shape> Display::shapes;

void Display::create_all () {
    Shape* poly = new Shape("Poly", Vector2(1, 2));

    poly->verts.push_back(Vector2(0, 0));
    poly->verts.push_back(Vector2(0, 3));
    poly->verts.push_back(Vector2(3, 5));
    poly->verts.push_back(Vector2(0, 0));

    Shape* square = new Shape("Square", Vector2(-1, -1));
    square->verts.push_back(Vector2(0, 0));
    square->verts.push_back(Vector2(0, 2));
    square->verts.push_back(Vector2(2, 2));
    square->verts.push_back(Vector2(2, 0));
    square->verts.push_back(Vector2(0, 0));

    Shape* wave = new Shape ("Sinewave", Vector2(-5, -5));

    for (int i = 0; i < 20; i++) {
        wave->verts.push_back(Vector2(i, sin(i)));
    }
        

    shapes.push_back(*poly);
    shapes.push_back(*square);
    shapes.push_back(*wave);
}
