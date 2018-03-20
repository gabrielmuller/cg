#include "shape.h"

Shape::Shape (std::string name, std::list<Vector2> verts) : 
	name(name), verts(verts) {}

Shape::Shape (std::string name) : 
	name(name) {}