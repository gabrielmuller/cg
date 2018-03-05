#ifndef VECTOR2_H
#define VECTOR2_H

#include <string>

struct Vector2 {
    float x;
    float y;

    Vector2();
    Vector2 (float x, float y); 

    operator std::string () const;

};

#endif
