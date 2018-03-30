#include "display.h"

#include "polygon.h"
#include "point.h"
#include "line.h"
#include <math.h>
#include "descobj.h"

std::list<Shape*> Display::shapes;

void Display::create_all () {

    Shape* poly = DescOBJ::read_obj("tri.obj");
    Shape* square = DescOBJ::read_obj("square.obj");

    Polygon* wave = new Polygon ("Sinewave");

    Point* point = new Point ("Point", 0, 0);
    Line* line = new Line ("Straight Line",
        {Vector2(1, -3), Vector2(3, -1)});

    for (int i = 0; i < 20; i++) {
        wave->verts.push_back(Vector2(i-5, sin(i)-5));
    }

    wave->open = true;

    shapes.push_back(poly);
    shapes.push_back(square);
    shapes.push_back(wave);
    shapes.push_back(line);
    shapes.push_back(point);
}

/*void Display::destroy_all() {
    shapes.clear();
}*/
void Display::draw_all(cairo_t* cr) {
    for (auto it = shapes.begin();
        it != shapes.end();
        ++it) {
        (*it)->draw(cr);
    }
}

void Display::add(Shape* sh) {
    shapes.push_back(sh);
}
