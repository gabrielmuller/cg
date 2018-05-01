#ifndef GUI_H
#define GUI_H

#include <iostream>
#include <vector>
#include <string>
#include <sstream> 
#include <unistd.h>

#include "vector2.h"
#include "display.h"
#include "window.h"
#include "vertices.h"
#include "polygon.h"
#include "spline.h"
#include "point.h"
#include "line.h"
#include "bezier.h"
#include "descobj.h"
#include "vector3.h"
#include "edge3d.h"

/**
 *  Funções de interface gráfica com GTK.
 */

struct Params {
    GtkWidget** entries;
    int type;
    Vertices* v;
};

namespace GUI {
    extern GtkWidget* drawing_area;
    extern GtkApplication* app;
    extern GtkWidget* combo;
    extern GtkTextBuffer* buffer;
    extern GtkTextIter iter;
    
    gboolean draw_cb(GtkWidget *widget, cairo_t* cr, gpointer* data);

    void set_cohen();
    void set_liang();

    void move (Vector2 amount);
    void move_z (float amount);
    void move_right ();
    void move_left ();
    void move_up ();
    void move_down ();
    void zoom_out ();
    void zoom_in ();
    void rotate_right ();
    void rotate_left ();

    void translation_cb(GtkWidget **entry, GtkWidget *widget);
    void translation_page(GtkWidget* frame);
    void scaling_cb(GtkWidget **entry, GtkWidget *widget);
    void scaling_page(GtkWidget* frame);
    void rotation_cb(GtkWidget **entry, GtkWidget *widget);
    void rotation_page(GtkWidget* frame);

    void add_point_cb(GtkWidget **entry, GtkWidget *widget);
    void create_point_frame ();

    void add_line_cb(GtkWidget **entry, GtkWidget *widget);
    void create_line_frame ();

    void on_create_poly_button(Params* p, GtkWidget *widget);
    void on_add_vert_button(GtkWidget **entry, GtkWidget *widget);
    void create_poly_frame ();
    void create_curve_frame ();

    void create_vertices_frame (bool isPoly);

    void on_create_shape_button();

    void add_3d_cb(GtkWidget **entry, GtkWidget *widget);
    void on_add_edge_button(GtkWidget **entry, GtkWidget *widget);
    void create_3d_frame();

    void on_import_button(GtkWidget *widget, GtkWidget *window);
    void on_export_button(GtkWidget *widget, GtkWidget *window);
    
    void activate(GtkApplication* app, gpointer user_data);
}

#endif
