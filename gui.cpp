#include "gui.h"

GtkWidget* GUI::drawing_area;
GtkApplication* GUI::app;
GtkWidget* GUI::combo;

// desenha no drawing area
gboolean GUI::draw_cb(GtkWidget *widget, cairo_t* cr, gpointer* data) {
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

void GUI::move (Vector2 amount) {
    Window::position = Window::position + amount;
    gtk_widget_queue_draw(drawing_area);
}
void GUI::move_z (float amount) {
    Window::size = Window::size - Vector2(amount, amount);
    gtk_widget_queue_draw(drawing_area);
}

void GUI::move_up () {
    GUI::move(Vector2(0, 1));
}
void GUI::move_down () {
    GUI::move(Vector2(0, -1));
}
void GUI::move_left () {
    GUI::move(Vector2(-1, 0));
}
void GUI::move_right () {
    GUI::move(Vector2(1, 0));
}

void GUI::zoom_in () {
    GUI::move_z(1);
}
void GUI::zoom_out () {
    GUI::move_z(-1);
}

/*****************************************
 *
 *   Janelas e callbacks de transformação
 *
 *****************************************/

void GUI::translation_cb(GtkWidget **entry, GtkWidget *widget) {
    auto coord_x = gtk_entry_get_text (GTK_ENTRY(entry[0]));
    auto coord_y = gtk_entry_get_text (GTK_ENTRY(entry[1]));

    auto selected_id = gtk_combo_box_get_active(GTK_COMBO_BOX(combo));
    auto it = Display::shapes.begin();
    std::advance(it, selected_id);
    (*it)->translation(std::stof(coord_x), std::stof(coord_y));

    gtk_widget_queue_draw(drawing_area);
}

void GUI::translation_page (GtkWidget* frame) {
    GtkWidget* grid;
    GtkWidget* ok_button;
    GtkWidget  *x_label, *y_label, *n_label;
    GtkWidget  *x_entry, *y_entry;

    // labels
    x_label = gtk_label_new("Coordenada x");
    y_label = gtk_label_new("Coordenada y");
    n_label = gtk_label_new("Vetor de translação");

    // Entries
    x_entry = gtk_entry_new();
    y_entry = gtk_entry_new();

    static GtkWidget *entries[2];
    entries[0] = x_entry;
    entries[1] = y_entry;

    // Botoes
    ok_button = gtk_button_new_with_label("Ok");
    g_signal_connect_swapped(ok_button, "clicked",
        G_CALLBACK (translation_cb), entries);

    grid = gtk_grid_new();
    //gtk_grid_set_row_spacing (GTK_GRID(grid), (guint)10);
    //gtk_grid_set_column_spacing (GTK_GRID(grid), (guint)10);

    // (grid, widget, coluna, linha, tamanhox, tamanhoy)
    gtk_grid_attach(GTK_GRID(grid), n_label, 0, 0, 2, 1);
    gtk_grid_attach(GTK_GRID(grid), x_label, 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), x_entry, 1, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), y_label, 0, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), y_entry, 1, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), ok_button, 0, 4, 2, 1);

    gtk_container_add(GTK_CONTAINER(frame), grid);
}

void GUI::scaling_cb(GtkWidget **entry, GtkWidget *widget) {
    auto coord_x = gtk_entry_get_text (GTK_ENTRY(entry[0]));
    auto coord_y = gtk_entry_get_text (GTK_ENTRY(entry[1]));

    auto selected_id = gtk_combo_box_get_active(GTK_COMBO_BOX(combo));
    auto it = Display::shapes.begin();
    std::advance(it, selected_id);
    (*it)->scaling(std::stof(coord_x), std::stof(coord_y));

    gtk_widget_queue_draw(drawing_area);
}

void GUI::scaling_page(GtkWidget* frame) {
    GtkWidget* grid;
    GtkWidget* ok_button;
    GtkWidget  *x_label, *y_label, *n_label;
    GtkWidget  *x_entry, *y_entry;

    // labels
    x_label = gtk_label_new("Sx");
    y_label = gtk_label_new("Sy");
    n_label = gtk_label_new("Fatores de escala");

    // Entries
    x_entry = gtk_entry_new();
    y_entry = gtk_entry_new();

    static GtkWidget *entries[2];
    entries[0] = x_entry;
    entries[1] = y_entry;

    // Botoes
    ok_button = gtk_button_new_with_label("Ok");
    g_signal_connect_swapped(ok_button, "clicked",
        G_CALLBACK (scaling_cb), entries);

    grid = gtk_grid_new();
    //gtk_grid_set_row_spacing (GTK_GRID(grid), (guint)10);
    //gtk_grid_set_column_spacing (GTK_GRID(grid), (guint)10);

    // (grid, widget, coluna, linha, tamanhox, tamanhoy)
    gtk_grid_attach(GTK_GRID(grid), n_label, 0, 0, 2, 1);
    gtk_grid_attach(GTK_GRID(grid), x_label, 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), x_entry, 1, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), y_label, 0, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), y_entry, 1, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), ok_button, 0, 4, 3, 1);

    gtk_container_add(GTK_CONTAINER(frame), grid);
}

void GUI::rotation_cb(GtkWidget **entry, GtkWidget *widget) {
    auto graus = gtk_entry_get_text (GTK_ENTRY(entry[0]));
    std::string dx = gtk_entry_get_text (GTK_ENTRY(entry[1]));
    std::string dy = gtk_entry_get_text (GTK_ENTRY(entry[2]));
    bool center = false;

    /* Se os campos x e y estiverem em branco, a rotação é
     * em relação ao centro geométrico
     */
    if (dx == "") {
        dx = "0";
        dy = "0";
        center = true;
    }
    auto selected_id = gtk_combo_box_get_active(GTK_COMBO_BOX(combo));
    auto it = Display::shapes.begin();
    std::advance(it, selected_id);
    (*it)->rotation(std::stof(dx), std::stof(dy), std::stof(graus), center);

    gtk_widget_queue_draw(drawing_area);
}

void GUI::rotation_page (GtkWidget* frame) {
    GtkWidget *grid, *ok_button, *d_label, *d_entry;
    GtkWidget *x_label, *y_label, *x_entry, *y_entry;
    GtkWidget *help_label, *help_frame;

    // labels
    d_label = gtk_label_new("Graus");
    x_label = gtk_label_new("Coordenada x");
    y_label = gtk_label_new("Coordenada y");
    help_frame = gtk_frame_new(" Ajuda");
    help_label = gtk_label_new(" Para rotação em relação ao centro do\n objeto, deixe os campos x, y em\n branco.");
    gtk_container_add(GTK_CONTAINER(help_frame), help_label);

    // Entries
    d_entry = gtk_entry_new();
    x_entry = gtk_entry_new();
    y_entry = gtk_entry_new();

    static GtkWidget *entries[3];
    entries[0] = d_entry;
    entries[1] = x_entry;
    entries[2] = y_entry;

    // Botoes
    ok_button = gtk_button_new_with_label("Ok");
    g_signal_connect_swapped(ok_button, "clicked",
        G_CALLBACK (rotation_cb), entries);

    grid = gtk_grid_new();
    //gtk_grid_set_row_spacing (GTK_GRID(grid), (guint)10);
    //gtk_grid_set_column_spacing (GTK_GRID(grid), (guint)10);

    // (grid, widget, coluna, linha, tamanhox, tamanhoy)
    gtk_grid_attach(GTK_GRID(grid), d_label, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), d_entry, 1, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), x_label, 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), x_entry, 1, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), y_label, 0, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), y_entry, 1, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), help_frame, 0, 3, 2, 2);
    gtk_grid_attach(GTK_GRID(grid), ok_button, 0, 5, 2, 1);

    gtk_container_add(GTK_CONTAINER(frame), grid);
}

/*****************************************
 *
 *  Callbacks pra adicionar ponto e reta
 *
 *****************************************/

void GUI::add_point_cb(GtkWidget **entry, GtkWidget *widget) {
    std::string nome = gtk_entry_get_text (GTK_ENTRY(entry[1]));
    auto coord_x = gtk_entry_get_text (GTK_ENTRY(entry[2]));
    auto coord_y = gtk_entry_get_text (GTK_ENTRY(entry[3]));

    Point* point = new Point (nome, {Vector2(std::stof(coord_x), 
        std::stof(coord_y))});
    Display::add(point);

    gtk_widget_queue_draw(drawing_area);
    gtk_combo_box_text_append_text (GTK_COMBO_BOX_TEXT (combo), nome.c_str());

    gtk_widget_destroy(GTK_WIDGET(entry[0]));
}

void GUI::add_line_cb(GtkWidget **entry, GtkWidget *widget) {
    std::string nome = gtk_entry_get_text (GTK_ENTRY(entry[1]));
    auto coord_x1 = gtk_entry_get_text (GTK_ENTRY(entry[2]));
    auto coord_y1 = gtk_entry_get_text (GTK_ENTRY(entry[3]));
    auto coord_x2 = gtk_entry_get_text (GTK_ENTRY(entry[4]));
    auto coord_y2 = gtk_entry_get_text (GTK_ENTRY(entry[5]));

    Line* line = new Line (nome, 
        {Vector2(std::stof(coord_x1), std::stof(coord_y1)), 
        Vector2(std::stof(coord_x2), std::stof(coord_y2))});
    Display::add(line);

    gtk_widget_queue_draw(drawing_area);
    gtk_combo_box_text_append_text (GTK_COMBO_BOX_TEXT (combo), nome.c_str());

    gtk_widget_destroy(GTK_WIDGET(entry[0]));
}

/*****************************************
 *
 *  Janelas pra adicionar ponto e reta
 *
 *****************************************/

void GUI::add_point_window () {
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
        G_CALLBACK (add_point_cb), entries);

    grid = gtk_grid_new();
    gtk_grid_set_row_spacing (GTK_GRID(grid), (guint)10);
    gtk_grid_set_column_spacing (GTK_GRID(grid), (guint)10);


    // coloca widgets nos containers
    gtk_container_add(GTK_CONTAINER(window), grid);

    // (grid, widget, coluna, linha, tamanhox, tamanhoy)
    gtk_grid_attach(GTK_GRID(grid), n_label, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), nome_entry, 1, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), x_label, 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), x_entry, 0, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), y_label, 1, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), y_entry, 1, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), ok_button, 0, 4, 3, 1);

    gtk_widget_show_all(window);
}

void GUI::add_line_window () {
    GtkWidget *window;
    GtkWidget *grid;
    GtkWidget *ok_button;
    GtkWidget *x_label, *y_label, *n_label;
    GtkWidget *p1_label, *p2_label;
    GtkWidget *x1_entry, *x2_entry, *y1_entry, *y2_entry;
    GtkWidget *nome_entry;

    // Window
    window = gtk_application_window_new (app);
    gtk_window_set_title (GTK_WINDOW (window), "Adicionar Reta");
    gtk_window_set_default_size (GTK_WINDOW(window), 100, 100);
    
    // labels
    x_label = gtk_label_new("Coordenada x");
    y_label = gtk_label_new("Coordenada y");
    n_label = gtk_label_new("Nome");
    p1_label = gtk_label_new("Vetor a");
    p2_label = gtk_label_new("Vetor b");

    // Entries
    nome_entry = gtk_entry_new();
    x1_entry = gtk_entry_new();
    y1_entry = gtk_entry_new();
    x2_entry = gtk_entry_new();
    y2_entry = gtk_entry_new();

    static GtkWidget *entries[6];
    entries[0] = window;
    entries[1] = nome_entry;
    entries[2] = x1_entry;
    entries[3] = y1_entry;
    entries[4] = x2_entry;
    entries[5] = y2_entry;

    // Botoes
    ok_button = gtk_button_new_with_label("Criar");
    g_signal_connect_swapped(ok_button, "clicked",
        G_CALLBACK (add_line_cb), entries);

    grid = gtk_grid_new();
    gtk_grid_set_row_spacing (GTK_GRID(grid), (guint)10);

    // coloca widgets nos containers
    gtk_container_add(GTK_CONTAINER(window), grid);

    // (grid, widget, coluna, linha, tamanhox, tamanhoy)
    gtk_grid_attach(GTK_GRID(grid), n_label, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), nome_entry, 1, 0, 1, 1);

    gtk_grid_attach(GTK_GRID(grid), p1_label, 0, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), p2_label, 0, 3, 1, 1);

    gtk_grid_attach(GTK_GRID(grid), x_label, 1, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), x1_entry, 1, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), x2_entry, 1, 3, 1, 1);

    gtk_grid_attach(GTK_GRID(grid), y_label, 2, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), y1_entry, 2, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), y2_entry, 2, 3, 1, 1);

    gtk_grid_attach(GTK_GRID(grid), ok_button, 0, 4, 3, 1);

    gtk_widget_show_all(window);
}

/*****************************************
 *
 *  Janela principal
 *
 *****************************************/

void GUI::activate (GtkApplication* app, gpointer user_data) {
    GUI::app = app;
    GtkWidget* window;
    GtkWidget *grid, *small_grid;
    GtkWidget *up_button, *down_button, *left_button, *right_button;
    GtkWidget *in_button, *out_button;
    GtkWidget *point_button, *line_button, *polygon_button; 
    
    Display::create_all();

    // cria janela
    window = gtk_application_window_new (app);
    gtk_window_set_title (GTK_WINDOW (window), "Demo CG");
    gtk_window_set_default_size (GTK_WINDOW(window), 480, 480);

    // cria botões de movimentação
    up_button = gtk_button_new_with_label("^");
    g_signal_connect_swapped(up_button, "clicked",
        G_CALLBACK (move_up), window);
        
    down_button = gtk_button_new_with_label("v");
    g_signal_connect_swapped(down_button, "clicked",
        G_CALLBACK (move_down), window);
    
    left_button = gtk_button_new_with_label("<");
    g_signal_connect_swapped(left_button, "clicked",
        G_CALLBACK (move_left), window);

    right_button = gtk_button_new_with_label(">");
    g_signal_connect_swapped(right_button, "clicked",
        G_CALLBACK (move_right), window);

    // cria grid de botões de movimentação
    small_grid = gtk_grid_new();
    // (grid, widget, coluna, linha, tamanhox, tamanhoy)
    gtk_grid_attach(GTK_GRID(small_grid), up_button, 1, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(small_grid), down_button, 1, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(small_grid), left_button, 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(small_grid), right_button, 2, 1, 1, 1);

    // cria botões de zoom
    in_button = gtk_button_new_with_label("Zoom in");
    g_signal_connect_swapped(in_button, "clicked",
        G_CALLBACK (zoom_in), window);

    out_button = gtk_button_new_with_label("Zoom out");
    g_signal_connect_swapped(out_button, "clicked",
        G_CALLBACK (zoom_out), window);

    // Criar botoes para criacao de objetos
    point_button = gtk_button_new_with_label("Adicionar Ponto");
    g_signal_connect_swapped(point_button, "clicked",
        G_CALLBACK (add_point_window), window);

    line_button = gtk_button_new_with_label("Adicionar Reta");
    g_signal_connect_swapped(line_button, "clicked",
        G_CALLBACK (add_line_window), window);

    polygon_button = gtk_button_new_with_label("Adicionar Poligono");
    /*g_signal_connect_swapped(polygon_button, "clicked",
        G_CALLBACK (add_poly_window), window);*/

    // Criar ComboBox
    combo = gtk_combo_box_text_new();
    for (auto it = Display::shapes.begin();
        it != Display::shapes.end(); ++it) {
        const char *textobjeto = (*it)->name.c_str();
        gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT (combo), textobjeto);
    }
    gtk_combo_box_set_active (GTK_COMBO_BOX (combo), 0);
    
    // ----------GtkNotebook
    GtkWidget *notebook, *frame, *label;
    notebook = gtk_notebook_new();
    gtk_notebook_set_tab_pos (GTK_NOTEBOOK(notebook), GTK_POS_TOP);
    
    frame = gtk_frame_new("");
    translation_page(frame);
    label = gtk_label_new ("Translação");
    gtk_notebook_append_page (GTK_NOTEBOOK (notebook), frame, label);
    frame = gtk_frame_new("");
    scaling_page(frame);
    label = gtk_label_new ("Escalonamento");
    gtk_notebook_append_page (GTK_NOTEBOOK (notebook), frame, label);
    frame = gtk_frame_new("");
    rotation_page(frame);
    label = gtk_label_new ("Rotação");
    gtk_notebook_append_page (GTK_NOTEBOOK (notebook), frame, label);
    // ---------GtkNotebook

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

    // (grid, widget, coluna, linha, tamanhox, tamanhoy)
    gtk_grid_attach(GTK_GRID(grid), small_grid, 1, 0, 2, 2);
    gtk_grid_attach(GTK_GRID(grid), in_button, 1, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), out_button, 2, 2, 1, 1);

    gtk_grid_attach(GTK_GRID(grid), point_button, 1, 4, 2, 1);
    gtk_grid_attach(GTK_GRID(grid), line_button, 1, 5, 2, 1);
    gtk_grid_attach(GTK_GRID(grid), polygon_button, 1, 6, 2, 1);

    gtk_grid_attach(GTK_GRID(grid), combo, 1, 7, 2, 1);
    gtk_grid_attach(GTK_GRID(grid), notebook, 1, 11, 2, 1);
    
    gtk_grid_attach(GTK_GRID(grid), drawing_area, 0, 0, 1, 12);

    gtk_widget_show_all(window);
}
