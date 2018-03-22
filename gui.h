#ifndef GUI_H
#define GUI_h

#include <iostream>
#include <vector>
#include <string>
#include <sstream> 

#include "display.h"
#include "window.h"
#include "polygon.h"
#include "point.h"
#include "line.h"

/**
 *  Funções de interface gráfica com GTK.
 */

namespace GUI {
    extern GtkWidget* drawing_area;
    extern GtkApplication* app;
    extern GtkWidget* combo;
    extern GtkTextBuffer* buffer;
    extern GtkTextIter iter;
    
    gboolean draw_cb(GtkWidget *widget, cairo_t* cr, gpointer* data);

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

    void add_poly_cb(GtkWidget *window, GtkWidget *widget);
    void add_vert_cb(GtkWidget **entry, GtkWidget *widget);
    void create_poly_cb(GtkWidget **entry, GtkWidget *widget);
    void add_verts_window (Polygon* poly);
    void add_poly_window ();
    
    void activate(GtkApplication* app, gpointer user_data);
}

#endif
