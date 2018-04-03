#include "display.h"

#include "polygon.h"
#include "point.h"
#include "line.h"
#include <math.h>
#include "descobj.h"

std::list<Shape*> Display::shapes;

void Display::create_all () {

    Shape* poly = DescOBJ::read_obj("Tri.obj");
    Shape* square = DescOBJ::read_obj("Square.obj");
    Polygon* trapezoid = dynamic_cast<Polygon*>(DescOBJ::read_obj("Trapezio.obj"));
    Polygon* fsquare = dynamic_cast<Polygon*>(DescOBJ::read_obj("Fsquare.obj"));
    Polygon* concavo = dynamic_cast<Polygon*>(DescOBJ::read_obj("Concavo.obj"));
    
    Polygon* wave = new Polygon ("Sinewave");

    Point* point = new Point ("Point", -1.5, 0);
    Line* line = new Line ("Straight Line",
        {Vector2(1, -3), Vector2(3, -1)});

    for (int i = 0; i < 20; i++) {
        wave->verts.push_back(Vector2(i-5, sin(i)-5));
    }

    wave->open = true;

    //trapezoid->fill = true;
    fsquare->fill = true;
    concavo->fill = true;
    
    shapes.push_back(poly);
    shapes.push_back(square);
    shapes.push_back(trapezoid);
    shapes.push_back(fsquare);
    shapes.push_back(concavo);
    shapes.push_back(wave);
    shapes.push_back(point);
    shapes.push_back(line);
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
