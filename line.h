#ifndef LINE_H
#define LINE_H

#include "polygon.h"
#include "vector2.h"

struct Line : Polygon {
	Line (std::string name, std::list<Vector2> verts);
};


#endif
