#ifndef GUI_H
#define GUI_h

#include <iostream>
#include <vector>
#include <string>

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
    
    void update_df_buffer(std::string nome);

    void add_reta_callback(GtkWidget **entry, GtkWidget *widget);
    void add_ponto_callback(GtkWidget **entry, GtkWidget *widget);

    void add_reta_window ();
    void add_ponto_window ();
    
    void activate(GtkApplication* app, gpointer user_data);
}

#endif
