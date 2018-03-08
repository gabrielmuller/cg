#include "polygon.h"
#include "point.h"
#include "line.h"
#include <math.h>
#include "display.h"

std::list<Shape*> Display::shapes;

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

    Point* point = new Point ("Point", Vector2(0, 0));
    Line* line = new Line ("Straight Line", Vector2(2, -2),
        Vector2(-1, -1), Vector2(1, 1));

    for (int i = 0; i < 20; i++) {
        wave->verts.push_back(Vector2(i, sin(i)));
    }
        

    shapes.push_back(poly);
    shapes.push_back(square);
    shapes.push_back(wave);
    shapes.push_back(line);
    shapes.push_back(point);
}

void Display::add(Shape* sh) {
    shapes.push_back(sh);
}