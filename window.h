#ifndef WINDOW_H
#define WINDOW_H

#include "AB.h"
#include <gtk/gtk.h>
#include <list>

enum {
    COHEN_SUTHERLAND,
    LIANG_BARSKY
};

enum {
    XL = 0,
    XR = 1,
    YD = 2,
    YU = 3
};

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
    extern int clipping_algorithm;
    extern cairo_t* cr;

    // extremidades da window
    extern float xr, xl, yu, yd;

    std::list<AB> edges();

    Vector2 world_to_screen (Vector2 coords);
    Vector2 world_to_norm (Vector2 coords);
    Vector2 norm_to_vp (Vector2 coords);

    void draw_line (AB line);
    void draw_pline (AB line);
    void draw_point (Vector2 point);

    bool is_inside(Vector2 coord, AB edge);
    
    Vector2 clip_to_edge(AB edge, AB line);
    AB clip_line (AB line);
    AB clip_cs (AB line);
    AB clip_lb (AB line);
    Vector2 clip_point (Vector2 point);
    void update_boundaries (); 
    int get_rc (Vector2 point); 
    void animate ();
};

#endif
