#include <gtk/gtk.h>
#include "shape.h"
#include <iostream>



// desenha no drawing area
gboolean draw_cb(GtkWidget *widget, cairo_t *cr, gpointer* data) {
    // fundo preto
    cairo_set_source_rgb(cr, 0, 0, 0);
    cairo_paint(cr);

    cairo_set_line_width(cr, 1);

    // linha branca
    cairo_set_source_rgb(cr, 1, 1, 1);

    //desenha duas retas
    /*cairo_move_to(cr, 0, 0);
    cairo_line_to(cr, 100, 100);
    cairo_line_to(cr, 100, 40);*/

    float zoom = 20;

    Shape* poly = (Shape*) data;

    std::list<Vector2>::iterator it = poly->verts.begin();

    cairo_move_to(cr, it->x * zoom, it->y * zoom);

    for (; it != poly->verts.end(); ++it) {
        cairo_line_to(cr, it->x * zoom, it->y * zoom);
    }
        
    cairo_stroke(cr);

    return FALSE;
}

static void activate (GtkApplication* app, gpointer user_data) {
    // polígono Poly na pos 2, 5 
    Shape* poly = new Shape("Poly", Vector2(2, 5));

    poly->verts.push_back(Vector2(0, 1));
    poly->verts.push_back(Vector2(0, 3));
    poly->verts.push_back(Vector2(3, 5));
    poly->verts.push_back(Vector2(0, 1));

    GtkWidget* window;
    GtkWidget* button;
    GtkWidget* drawing_area;
    GtkWidget* grid;
    
    // cria janela
    window = gtk_application_window_new (app);
    gtk_window_set_title (GTK_WINDOW (window), "Demo CG");
    gtk_window_set_default_size (GTK_WINDOW(window), 320, 240);
    
    // cria botão
    button = gtk_button_new_with_label("TOP");
    g_signal_connect_swapped(button, "clicked",
        G_CALLBACK (gtk_widget_destroy), window);
        
    // cria drawing area
    drawing_area = gtk_drawing_area_new();
    gtk_widget_set_size_request(drawing_area, 200, 200);
    g_signal_connect(G_OBJECT(drawing_area), "draw",
        G_CALLBACK(draw_cb), (gpointer*) poly);

    //cria grid
    grid = gtk_grid_new();

    // coloca widgets nos containers
    gtk_container_add(GTK_CONTAINER(window), grid);
    gtk_grid_attach(GTK_GRID(grid), button, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), drawing_area, 1, 1, 2, 2);


    gtk_widget_show_all(window);
}

int main (int argc, char **argv) {

    // inicia app
    GtkApplication *app;
    int status;

    app = gtk_application_new ("org.gtk.cg", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;
}
