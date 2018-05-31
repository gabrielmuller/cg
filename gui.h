#ifndef GUI_H
#define GUI_H

#include <iostream>
#include <string>
#include <sstream> 
#include <unistd.h>
#include <vector>

#include "bezier.h"
#include "descobj.h"
#include "display.h"
#include "edge3d.h"
#include "line.h"
#include "point.h"
#include "polygon.h"
#include "rotation.h"
#include "spline.h"
#include "vertices.h"
#include "vector2.h"
#include "vector3.h"
#include "window.h"

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
    
    gboolean draw_cb (GtkWidget *widget, cairo_t* cr, gpointer* data);

    void set_cohen ();
    void set_liang ();
    void set_2D ();
    void set_cavalier ();
    void set_cam_perspective ();
    void set_rotation_axis_x ();
    void set_rotation_axis_y ();
    void set_rotation_axis_z ();
    void set_shape_axis_x ();
    void set_shape_axis_y ();
    void set_shape_axis_z ();

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
    void rotate_cam (const bool right);
    void set_perspective (GtkAdjustment *perspective_scale);

    /*  Transformações  */
    void on_translate_button (GtkWidget **entry, GtkWidget *widget);
    void on_scale_button (GtkWidget **entry, GtkWidget *widget);
    void on_rotate_button (GtkWidget **entry, GtkWidget *widget);
    void translation_page (GtkWidget* frame);
    void scaling_page (GtkWidget* frame);
    void rotation_page (GtkWidget* frame);

    /*   Criar figura 2D    */
    void on_finish_2D_button (Params* p, GtkWidget *widget);
    void on_add_vert_button (GtkWidget **entry, GtkWidget *widget);
    void on_create_2D_button ();
    void on_create_curve_button ();
    void create_vertices_frame (bool isPoly);

    /*   Criar figura 3D    */
    void on_finish_3D_button (GtkWidget **entry, GtkWidget *widget);
    void on_add_edge_button (GtkWidget **entry, GtkWidget *widget);
    void on_create_3D_button ();

    /*  Importar/exportar figura  */
    void on_import_button (GtkWidget *widget, GtkWidget *window);
    void on_export_button (GtkWidget *widget, GtkWidget *window);
    
    void activate (GtkApplication* app, gpointer user_data);
}

#endif
