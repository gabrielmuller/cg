#include "line.h"

Line::Line (std::string name, std::list<Vector2> verts) : 
	Polygon(name, verts) {
        open = true;
        fill = false;
    }