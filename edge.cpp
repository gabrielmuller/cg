#include "edge.h"

Edge::Edge () : empty(true) {}
Edge::Edge (Vector2 a, Vector2 b) : a(a), b(b), empty(false) {}
