#ifndef WINDOW_H
#define WINDOW_H

#include "vector2.h"

namespace Window {
    extern Vector2 position;
    extern Vector2 viewport;
    extern Vector2 size;
    extern float angle;
    Vector2 world_to_screen (Vector2 coords);
    Vector2 world_to_norm (Vector2 coords);
    Vector2 norm_to_vp (Vector2 coords);
};

#endif
