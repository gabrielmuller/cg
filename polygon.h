#ifndef POLYGON_H
#define POLYGON_H

#include <string>
#include <list>
#include "vector2.h"


struct Polygon {
    std::string name;
    Vector2 position;
    std::list<Vector2> verts;

    Polygon (std::string name, Vector2 position); 
};

#endif
