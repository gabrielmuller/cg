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
#include "polygon.h"
#include "point.h"
#include "line.h"
#include "bezier.h"
#include "descobj.h"

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
    void add_point_window ();

    void add_line_cb(GtkWidget **entry, GtkWidget *widget);
    void add_line_window ();

    void add_poly_cb(Params* p, GtkWidget *widget);
    void add_vert_cb(Params* p, GtkWidget *widget);
    void create_poly_cb(Params* p, GtkWidget *widget);
    void add_verts_window (Vertices* poly);
    void add_poly_window (Params* t);

    void create_shape();

    void on_import_button(GtkWidget *widget, GtkWidget *window);
    void on_export_button(GtkWidget *widget, GtkWidget *window);
    
    void activate(GtkApplication* app, gpointer user_data);
}

#endif
