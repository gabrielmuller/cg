#include <iostream>
#include <vector>
#include <string>

#include "display.h"
#include "window.h"
#include "ListaEnc.hpp"
#include "polygon.h"
#include "point.h"
#include "line.h"

GtkWidget* drawing_area;
GtkApplication *app;
GtkTextBuffer *buffer;
GtkTextIter iter;

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

/**
 *  Atualiza o GtkBuffer do GtkTextView que mostra os nomes
 *  dos objetos na janela principal
 */
void update_df_buffer(std::string nome) {
    nome.append("\n");
    gtk_text_buffer_get_end_iter(buffer, &iter);
    gtk_text_buffer_insert(buffer, &iter,
        nome.c_str(), -1);
}

/*****************************************
 *
 *  Callbacks pra adicionar ponto e reta
 *
 *****************************************/

static void add_ponto_callback(GtkWidget **entry, GtkWidget *widget) {
    auto nome = gtk_entry_get_text (GTK_ENTRY(entry[1]));
    auto coord_x = gtk_entry_get_text (GTK_ENTRY(entry[2]));
    auto coord_y = gtk_entry_get_text (GTK_ENTRY(entry[3]));

    Point* point = new Point (nome, Vector2(std::stof(coord_x), 
        std::stof(coord_y)));
    Display::add(point);

    g_signal_connect (G_OBJECT (drawing_area), "draw",
                    G_CALLBACK (draw_cb), NULL);
    update_df_buffer(nome);

    gtk_widget_destroy(GTK_WIDGET(entry[0]));
}

static void add_reta_callback(GtkWidget **entry, GtkWidget *widget) {
    auto nome = gtk_entry_get_text (GTK_ENTRY(entry[1]));
    auto coord_x0 = gtk_entry_get_text (GTK_ENTRY(entry[2]));
    auto coord_y0 = gtk_entry_get_text (GTK_ENTRY(entry[3]));
    auto coord_x1 = gtk_entry_get_text (GTK_ENTRY(entry[4]));
    auto coord_y1 = gtk_entry_get_text (GTK_ENTRY(entry[5]));
    auto coord_x2 = gtk_entry_get_text (GTK_ENTRY(entry[6]));
    auto coord_y2 = gtk_entry_get_text (GTK_ENTRY(entry[7]));

    Line* line = new Line (nome, 
        Vector2(std::stof(coord_x0), std::stof(coord_y0)),
        Vector2(std::stof(coord_x1), std::stof(coord_y1)), 
        Vector2(std::stof(coord_x2), std::stof(coord_y2)));
    Display::add(line);

    g_signal_connect (G_OBJECT (drawing_area), "draw",
                    G_CALLBACK (draw_cb), NULL);
    update_df_buffer(nome);

    gtk_widget_destroy(GTK_WIDGET(entry[0]));
}

/*****************************************
 *
 *  Janelas pra adicionar ponto e reta
 *
 *****************************************/

static void add_ponto_window () {
    GtkWidget* window;
    GtkWidget* grid;
    GtkWidget* ok_button;
    GtkWidget  *x_label, *y_label, *n_label;
    GtkWidget  *x_entry, *y_entry, *nome_entry;

    // Window
    window = gtk_application_window_new (app);
    gtk_window_set_title (GTK_WINDOW (window), "Adicionar Ponto");
    gtk_window_set_default_size (GTK_WINDOW(window), 100, 100);

    // labels
    x_label = gtk_label_new("Coordenada x");
    y_label = gtk_label_new("Coordenada y");
    n_label = gtk_label_new("Nome");

    // Entries
    x_entry = gtk_entry_new();
    y_entry = gtk_entry_new();
    nome_entry = gtk_entry_new();

    static GtkWidget *entries[4];
    entries[0] = window;
    entries[1] = nome_entry;
    entries[2] = x_entry;
    entries[3] = y_entry;

    // Botoes
    ok_button = gtk_button_new_with_label("Criar");
    g_signal_connect_swapped(ok_button, "clicked",
        G_CALLBACK (add_ponto_callback), entries);

    grid = gtk_grid_new();

    // coloca widgets nos containers
    gtk_container_add(GTK_CONTAINER(window), grid);

    gtk_grid_attach(GTK_GRID(grid), n_label, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), nome_entry, 1, 0, 1, 1);

    gtk_grid_attach(GTK_GRID(grid), x_label, 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), x_entry, 0, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), y_label, 1, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), y_entry, 1, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), ok_button, 0, 4, 3, 1);

    gtk_widget_show_all(window);
}

static void add_reta_window () {
    GtkWidget* window;
    GtkWidget* grid;
    GtkWidget* ok_button;
    GtkWidget  *x_label, *y_label, *n_label;
    GtkWidget *p0_label, *p1_label, *p2_label;
    GtkWidget *x0_entry, *x1_entry, *x2_entry;
    GtkWidget *y0_entry, *y1_entry, *y2_entry;
    GtkWidget *nome_entry;

    // Window
    window = gtk_application_window_new (app);
    gtk_window_set_title (GTK_WINDOW (window), "Adicionar Reta");
    gtk_window_set_default_size (GTK_WINDOW(window), 100, 100);
    
    // labels
    x_label = gtk_label_new("Coordenada x");
    y_label = gtk_label_new("Coordenada y");
    n_label = gtk_label_new("Nome");

    p0_label = gtk_label_new("Posição");
    p1_label = gtk_label_new("Vetor a");
    p2_label = gtk_label_new("Vetor b");

    // Entries
    x0_entry = gtk_entry_new();
    y0_entry = gtk_entry_new();
    x1_entry = gtk_entry_new();
    y1_entry = gtk_entry_new();
    x2_entry = gtk_entry_new();
    y2_entry = gtk_entry_new();
    nome_entry = gtk_entry_new();

    static GtkWidget *entries[8];
    entries[0] = window;
    entries[1] = nome_entry;
    entries[2] = x0_entry;
    entries[3] = y0_entry;
    entries[4] = x1_entry;
    entries[5] = y1_entry;
    entries[6] = x2_entry;
    entries[7] = y2_entry;

    // Botoes
    ok_button = gtk_button_new_with_label("Criar");
    g_signal_connect_swapped(ok_button, "clicked",
        G_CALLBACK (add_reta_callback), entries);

    grid = gtk_grid_new();

    // coloca widgets nos containers
    gtk_container_add(GTK_CONTAINER(window), grid);

    gtk_grid_attach(GTK_GRID(grid), n_label, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), nome_entry, 1, 0, 1, 1);

    gtk_grid_attach(GTK_GRID(grid), p0_label, 0, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), p1_label, 0, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), p2_label, 0, 4, 1, 1);

    gtk_grid_attach(GTK_GRID(grid), x_label, 1, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), x0_entry, 1, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), x1_entry, 1, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), x2_entry, 1, 4, 1, 1);

    gtk_grid_attach(GTK_GRID(grid), y_label, 2, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), y0_entry, 2, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), y1_entry, 2, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), y2_entry, 2, 4, 1, 1);

    gtk_grid_attach(GTK_GRID(grid), ok_button, 0, 5, 3, 1);

    gtk_widget_show_all(window);
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

    GtkWidget* ponto_button;
    GtkWidget* reta_button;
    GtkWidget* poligono_button;
    GtkWidget* df_text_view;
    
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

    // Criar botoes para criacao de objetos
    ponto_button = gtk_button_new_with_label("Adicionar Ponto");
    g_signal_connect_swapped(ponto_button, "clicked",
        G_CALLBACK (add_ponto_window), window);

    reta_button = gtk_button_new_with_label("Adicionar Reta");
    g_signal_connect_swapped(reta_button, "clicked",
        G_CALLBACK (add_reta_window), window);

    poligono_button = gtk_button_new_with_label("Adicionar Poligono");
    /*g_signal_connect_swapped(poligono_button, "clicked",
        G_CALLBACK (add_poligono_window), window);*/

    // Criar TextView
    df_text_view = gtk_text_view_new();
    gtk_text_view_set_editable (GTK_TEXT_VIEW (df_text_view), FALSE);
    buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (df_text_view));
 
    // Inicializa GtkTextBuffer da GtkTextView com os nomes
    // dos objetos do DisplayFile
    for (auto it = Display::shapes.begin();
        it != Display::shapes.end(); ++it) {
        std::string textobjeto = (*it)->name;
        textobjeto.append("\n");
        gtk_text_buffer_get_end_iter(buffer, &iter);
        gtk_text_buffer_insert(buffer, &iter,
            textobjeto.c_str(), -1);
    }
    
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

    gtk_grid_attach(GTK_GRID(grid), ponto_button, 2, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), reta_button, 2, 4, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), poligono_button, 2, 5, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), df_text_view, 2, 6, 1, 1);
    
    gtk_grid_attach(GTK_GRID(grid), drawing_area, 0, 3, 2, 4);


    gtk_widget_show_all(window);
}

int main (int argc, char **argv) {

    //teste
    /*ListaEnc<int>* lista = new ListaEnc<int>();
    lista->adiciona(5);
    lista->adiciona(50);
    lista->adiciona(40);
    std::cout << lista->end()->getProximo()->getInfo() << std::endl;

    // Precisa arrumar o end() e posicaoMem()
    for (auto it = lista->begin();
        it != lista->end();
        it = it->getProximo()) {
        std::cout << it->getInfo() << std::endl;
    }*/

    // inicia app
    //GtkApplication *app;
    int status;

    app = gtk_application_new ("org.gtk.cg", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;
}
