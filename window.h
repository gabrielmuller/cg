#ifndef WINDOW_H
#define WINDOW_H

#include "AB.h"

struct Specs {
    Vector2 position;
    Vector2 size;
    float angle;
    Specs();
};

namespace Window {
    extern Specs real;
    extern Specs goal;
    extern Vector2 viewport;
    extern float smooth;

    // extremidades da window
    extern float xr, xl, yu, yd;

    Vector2 world_to_screen (Vector2 coords);
    Vector2 world_to_norm (Vector2 coords);
    Vector2 norm_to_vp (Vector2 coords);
    AB clip_line (AB line);
    void update_boundaries (); 
    int get_rc (Vector2 point); 
    void animate ();
};

#endif
