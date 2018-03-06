#ifndef CAMERA_H
#define CAMERA_H

#include "vector2.h"

namespace Camera {
    extern Vector2 position;
    extern Vector2 viewport;
    extern Vector2 size;
    Vector2 world_to_screen(Vector2 coords);
};

#endif
