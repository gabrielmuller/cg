#ifndef POINT_H
#define POINT_H

#include <list>
#include "vector2.h"
#include "shape.h"


struct Point : Shape {
    Point (std::string name, Vector2 position); 
};

#endif
