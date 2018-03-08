#include <iostream>
#include "display.h"
#include "window.h"
#include "ListaEnc.hpp"


GtkWidget* drawing_area;
    

// desenha no drawing area
gboolean draw_cb(GtkWidget *widget, cairo_t* cr, gpointer* data) {
    // fundo preto
    cairo_set_source_rgb(cr, 0, 0, 0);
    cairo_paint(cr);

    cairo_set_line_width(cr, 1);

    // linha branca
    cairo_set_source_rgb(cr, 1, 1, 1);

    for (auto it = Display::shapes.begin();
        it != Display::shapes.end();
        ++it) {
        (*it)->draw(cr);
    }

    return FALSE;
}

static void move (Vector2 amount) {
    Window::position = Window::position + amount;
    gtk_widget_queue_draw(drawing_area);
}
static void move_z (float amount) {
    Window::size = Window::size - Vector2(amount, amount);
    gtk_widget_queue_draw(drawing_area);
}

static void move_up () {
    move(Vector2(0, 1));
}
static void move_down () {
    move(Vector2(0, -1));
}
static void move_left () {
    move(Vector2(-1, 0));
}
static void move_right () {
    move(Vector2(1, 0));
}

static void zoom_in () {
    move_z(1);
}
static void zoom_out () {
    move_z(-1);
}


static void activate (GtkApplication* app, gpointer user_data) {
    GtkWidget* window;
    GtkWidget* grid;

    GtkWidget* up_button;
    GtkWidget* down_button;
    GtkWidget* left_button;
    GtkWidget* right_button;

    GtkWidget* in_button;
    GtkWidget* out_button;

    Display::create_all();


    // cria janela
    window = gtk_application_window_new (app);
    gtk_window_set_title (GTK_WINDOW (window), "Demo CG");
    gtk_window_set_default_size (GTK_WINDOW(window), 480, 480);
    
    // cria botões
    up_button = gtk_button_new_with_label("Cima");
    g_signal_connect_swapped(up_button, "clicked",
        G_CALLBACK (move_up), window);
        
    down_button = gtk_button_new_with_label("Baixo");
    g_signal_connect_swapped(down_button, "clicked",
        G_CALLBACK (move_down), window);
    
    left_button = gtk_button_new_with_label("Esquerda");
    g_signal_connect_swapped(left_button, "clicked",
        G_CALLBACK (move_left), window);

    right_button = gtk_button_new_with_label("Direita");
    g_signal_connect_swapped(right_button, "clicked",
        G_CALLBACK (move_right), window);

    in_button = gtk_button_new_with_label("Aumentar");
    g_signal_connect_swapped(in_button, "clicked",
        G_CALLBACK (zoom_in), window);

    out_button = gtk_button_new_with_label("Diminuir");
    g_signal_connect_swapped(out_button, "clicked",
        G_CALLBACK (zoom_out), window);

    // cria drawing area
    drawing_area = gtk_drawing_area_new();
    gtk_widget_set_size_request(drawing_area, 
        Window::viewport.x, Window::viewport.y);
    g_signal_connect(G_OBJECT(drawing_area), "draw",
        G_CALLBACK(draw_cb), nullptr);

    //cria grid
    grid = gtk_grid_new();

    // coloca widgets nos containers
    gtk_container_add(GTK_CONTAINER(window), grid);
    gtk_grid_attach(GTK_GRID(grid), up_button, 1, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), down_button, 1, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), left_button, 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), right_button, 2, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), in_button, 2, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), out_button, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), drawing_area, 0, 3, 2, 2);


    gtk_widget_show_all(window);
}

int main (int argc, char **argv) {

    //teste
    /*ListaEnc<int>* lista = new ListaEnc<int>();
    lista->adiciona(5);
    lista->adiciona(50);
    lista->adiciona(40);
    std::cout << lista->end()->getProximo()->getInfo() << std::endl;

    // Precisa arrumar o end()
    for (auto it = lista->begin();
        it != lista->end();
        it = it->getProximo()) {
        std::cout << it->getInfo() << std::endl;
    }*/

    // inicia app
    GtkApplication *app;
    int status;

    app = gtk_application_new ("org.gtk.cg", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;
}
