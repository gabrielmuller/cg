#include "display.h"

#include "bezier.h"
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
    
    Bezier* curve = new Bezier ("Curva",
        {Vector2(-6, 0), Vector2(-6, -1), Vector2(-8, -1), Vector2(-8, 0),
         Vector2(-18, -1)/*, Vector2(-19, -3), Vector2(-23, -5)*/});

    Point* point = new Point ("Point", -1.5, 0);
    Line* line = new Line ("Straight Line",
        {Vector2(1, -3), Vector2(3, -1)});


    trapezoid->fill = true;
    fsquare->fill = true;
    concavo->fill = true;
    
    shapes.push_back(poly);
    shapes.push_back(square);
    shapes.push_back(trapezoid);
    shapes.push_back(fsquare);
    shapes.push_back(concavo);
    shapes.push_back(point);
    shapes.push_back(line);
    shapes.push_back(curve);
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
