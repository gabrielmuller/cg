#ifndef SHAPE_H
#define SHAPE_H

#include <string>
#include <list>
#include "vector2.h"


struct Shape {
    std::string name;
    Vector2 position;
    std::list<Vector2> verts;

    Shape (std::string name, Vector2 position); 
};

#endif
