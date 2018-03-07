#include "shape.h"
#include "vector2.h"

struct Line : Shape {
	extern Vector2 a;
	extern Vector2 b;
	Line (Vector2 a, Vector2 b);
};

