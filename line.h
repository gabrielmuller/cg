#ifndef LINE_H
#define LINE_H

#include "polygon.h"
#include "vector2.h"

struct Line : Polygon {
	Line (std::string name, Vector2 position, Vector2 a, Vector2 b);
};


#endif
