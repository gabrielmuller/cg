#ifndef WINDOW_H
#define WINDOW_H

#include <gtk/gtk.h>
#include <list>
#include "math.h"

#include "edge.h"
#include "edge3d.h"
#include "transformation.h"

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

struct Specs3D {
    Vector3 position;
    Vector2 orthoSize;
    float fov;
    Vector3 forward;
    Specs3D();
};

namespace Window {
    extern Specs real;
    extern Specs goal;
    extern Specs3D real3;
    extern Vector2 viewport;
    extern float smooth;
    extern int clipping_algorithm;
    extern cairo_t* cr;
    extern float clip_margin;

    // extremidades da window
    extern float xr, xl, yu, yd;

    std::list<Edge> edges();

    Vector2 world_to_screen (Vector2 coords);
    Vector2 world_to_norm (Vector2 coords);
    Vector2 world_to_norm (Vector3 coords);
    Vector2 norm_to_vp (Vector2 coords);

    void draw_point (Vector2 point);
    void draw_point (Vector3 point);
    void draw_line (Edge line);
    void draw_line (Edge3D line);
    void draw_pline (Edge line);
    
    Edge clip_line (Edge line);
    Edge clip_cs (Edge line);
    Edge clip_lb (Edge line);
    Vector2 clip_to_edge(Edge edge, Edge line);
    bool is_inside(Vector2 coord, Edge edge);
    int get_rc (Vector2 point); 

    void draw_borders ();
    void update_boundaries (); 

    void animate ();
};

#endif
