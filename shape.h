#ifndef SHAPE_H
#define SHAPE_H

#include <string>
#include "vector2.h"

struct Shape {
    std::string name;
    Vector2 position;

    Shape (std::string name, Vector2 position); 
};



#endif
