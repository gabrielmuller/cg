#include "gui.h"

GtkWidget* GUI::drawing_area;
GtkApplication* GUI::app;
GtkWidget* GUI::combo;
GtkTextBuffer* GUI::buffer;
GtkTextIter GUI::iter;
std::vector<Vector2> vert_buffer_list;

enum {
    POLYGON,
    CURVE
};

// desenha no drawing area
gboolean GUI::draw_cb(GtkWidget *widget, cairo_t* cr, gpointer* data) {
    // fundo preto
    cairo_set_source_rgb(cr, 0, 0, 0);
    cairo_paint(cr);

    cairo_set_line_width(cr, 1);

    // linha branca
    cairo_set_source_rgb(cr, 1, 1, 1);

    // ?
    Window::cr = cr;

    Display::draw_all();
    return FALSE;
}

void GUI::set_cohen() {
    Window::clipping_algorithm = COHEN_SUTHERLAND;
}

void GUI::set_liang() {
    Window::clipping_algorithm = LIANG_BARSKY;
}

void GUI::move (Vector2 amount) {
    Transformation rot = Transformation::rotation(-Window::goal.angle, Vector2(0, 0));
    amount = (Transformation) amount * rot;
    Window::goal.position = Window::goal.position + amount;
    //gtk_widget_queue_draw(drawing_area);
}
void GUI::move_z (float amount) {
    if (Window::goal.size.x() > amount && 
        Window::goal.size.y() > amount) {
        Window::goal.size = Window::goal.size - Vector2(amount, amount);
    }

    //gtk_widget_queue_draw(drawing_area);
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

void GUI::rotate_right() {
    Window::goal.angle += 0.1;
    //gtk_widget_queue_draw(drawing_area);
}

void GUI::rotate_left() {
    Window::goal.angle -= 0.1;
    //gtk_widget_queue_draw(drawing_area);
}

/*****************************************
 *
 *   "Páginas" ou frames 
 *   e callbacks de transformação
 *
 *****************************************/

void GUI::translation_cb(GtkWidget **entry, GtkWidget *widget) {
    std::string coord_x = gtk_entry_get_text (GTK_ENTRY(entry[0]));
    std::string coord_y = gtk_entry_get_text (GTK_ENTRY(entry[1]));

    if (coord_x == "") coord_x = "0";
    if (coord_y == "") coord_y = "0";

    auto selected_id = gtk_combo_box_get_active(GTK_COMBO_BOX(combo));
    auto it = Display::shapes.begin();
    std::advance(it, selected_id);
    (*it)->translate(Vector2(std::stof(coord_x), std::stof(coord_y)));

    //gtk_widget_queue_draw(drawing_area);
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
    std::string coord_x = gtk_entry_get_text (GTK_ENTRY(entry[0]));
    std::string coord_y = gtk_entry_get_text (GTK_ENTRY(entry[1]));

    if (coord_x == "") coord_x = "1";
    if (coord_y == "") coord_y = "1";

    auto selected_id = gtk_combo_box_get_active(GTK_COMBO_BOX(combo));
    auto it = Display::shapes.begin();
    std::advance(it, selected_id);
    (*it)->scale(Vector2(std::stof(coord_x), std::stof(coord_y)));

    //gtk_widget_queue_draw(drawing_area);
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

    float rad = std::stof(graus) * (M_PI/180);
    if (center) {
        (*it)->rotate(rad);
    } else {
        (*it)->rotate(rad, Vector2(std::stof(dx), std::stof(dy)));
    }

    g_signal_connect (G_OBJECT (drawing_area), "draw",
                    G_CALLBACK (draw_cb), NULL);
    //gtk_widget_queue_draw(drawing_area);
}

void GUI::rotation_page (GtkWidget* frame) {
    GtkWidget *grid, *ok_button, *d_label, *d_entry;
    GtkWidget *x_label, *y_label, *x_entry, *y_entry;

    // labels
    d_label = gtk_label_new("Graus");
    x_label = gtk_label_new("Coordenada x");
    y_label = gtk_label_new("Coordenada y");

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

    gtk_widget_set_tooltip_text (GTK_WIDGET(ok_button),
        "Para rotação em relação ao centro do\n objeto, deixe os campos x, y em\n branco.");
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
    gtk_grid_attach(GTK_GRID(grid), ok_button, 0, 3, 2, 1);

    gtk_container_add(GTK_CONTAINER(frame), grid);
}

/*****************************************
 *
 *  Janela e callbacks pra criar ponto 
 *
 *****************************************/

void GUI::add_point_cb(GtkWidget **entry, GtkWidget *widget) {
    std::string nome = gtk_entry_get_text (GTK_ENTRY(entry[1]));
    auto coord_x = gtk_entry_get_text (GTK_ENTRY(entry[2]));
    auto coord_y = gtk_entry_get_text (GTK_ENTRY(entry[3]));

    Point* point = new Point (nome, std::stof(coord_x), std::stof(coord_y));
    Display::add(point);
    //gtk_widget_queue_draw(drawing_area);
    gtk_combo_box_text_append_text (GTK_COMBO_BOX_TEXT (combo), nome.c_str());
    gtk_widget_destroy(GTK_WIDGET(entry[0]));
}

void GUI::create_point_frame () {
    GtkWidget *window, *grid;
    GtkWidget *ok_button;
    GtkWidget *x_label, *y_label, *n_label;
    GtkWidget *x_entry, *y_entry, *nome_entry;

    window = gtk_application_window_new (app);
    gtk_window_set_title (GTK_WINDOW (window), "Adicionar Ponto");

    // labels
    n_label = gtk_label_new("Nome");
    x_label = gtk_label_new("Coordenada x");
    y_label = gtk_label_new("Coordenada y");
    
    // Entries
    nome_entry = gtk_entry_new();
    x_entry = gtk_entry_new();
    y_entry = gtk_entry_new();

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

/*****************************************
 *
 *  Janela e callbacks pra criar linha 
 *
 *****************************************/

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
    //gtk_widget_queue_draw(drawing_area);
    gtk_combo_box_text_append_text (GTK_COMBO_BOX_TEXT (combo), nome.c_str());
    gtk_widget_destroy(GTK_WIDGET(entry[0]));
}

void GUI::create_line_frame () {
    GtkWidget *window, *grid;
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
    n_label = gtk_label_new("Nome");
    x_label = gtk_label_new("Coordenada x");
    y_label = gtk_label_new("Coordenada y");
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
 *  Frame e callbacks para criar polígono
 *  ou curva de Bezier
 *
 *****************************************/

// Termina criação de polígono/curva e desenha na tela
void GUI::on_create_poly_button(Params* p, GtkWidget *widget) {
    std::string name = gtk_entry_get_text (GTK_ENTRY(p->entries[1]));
    Shape *sh;
    if (p->type == POLYGON) {
        bool fill = gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(p->entries[2]));
        if (vert_buffer_list.size() == 1)
            sh = new Point(name, vert_buffer_list.front().x(),
                vert_buffer_list.front().y());
        else if (vert_buffer_list.size() == 2)
            sh = new Line(name, vert_buffer_list);
        else if (vert_buffer_list.size() >= 3)
            sh = new Polygon(name, vert_buffer_list, fill);
    } else {
        bool type = gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(p->entries[2]));
        if (!type) sh = new Bezier(name, vert_buffer_list);
        else sh = new Spline(name, vert_buffer_list);
    }
    Display::add(sh);
    //gtk_widget_queue_draw(drawing_area);
    gtk_combo_box_text_append_text (GTK_COMBO_BOX_TEXT (combo), name.c_str());
    gtk_widget_destroy(GTK_WIDGET(p->entries[0]));
}

// Adiciona vértice ao polígono/curva
void GUI::on_add_vert_button(GtkWidget **entry, GtkWidget *widget) {
    auto coord_x = gtk_entry_get_text (GTK_ENTRY(entry[0]));
    auto coord_y = gtk_entry_get_text (GTK_ENTRY(entry[1]));
    vert_buffer_list.push_back(Vector2(std::stof(coord_x), std::stof(coord_y)));
    // Simplificar isso depois
    std::ostringstream s;
    s << "(" << coord_x << ", " << coord_y << ")";
    std::string ss = s.str(); //
    gtk_text_buffer_get_end_iter(buffer, &iter);
    gtk_text_buffer_insert(buffer, &iter, ss.c_str(), -1);
}

// Janela para criar polígono e adicionar vértices
void GUI::create_poly_frame() {
    GtkWidget *window, *grid;
    GtkWidget *add_vert_b, *add_poly_b;
    GtkWidget *x_label, *y_label, *t1_label, *t2_label;
    GtkWidget *x_entry, *y_entry;
    GtkWidget *name_entry, *name_label, *fill_check;

    // Window
    window = gtk_application_window_new (app);
    gtk_window_set_title (GTK_WINDOW (window), "Adicionar Polígono");

    //std::list<Vector2> vert_buffer_list;
    // labels   
    name_label = gtk_label_new("Nome");
    t1_label = gtk_label_new("Adicionar vértice");
    t2_label = gtk_label_new("Vértices");
    x_label = gtk_label_new("Posição x");
    y_label = gtk_label_new("Posição y");
    
    // Entries
    name_entry = gtk_entry_new();
    fill_check = gtk_check_button_new_with_label("Preencher?");
    x_entry = gtk_entry_new();
    y_entry = gtk_entry_new();

    static GtkWidget *entriesA[2];
    entriesA[0] = x_entry;
    entriesA[1] = y_entry;

    static GtkWidget *entriesB[5];
    entriesB[0] = window;
    entriesB[1] = name_entry;
    entriesB[2] = fill_check;
    Params* p = new Params();
    p->entries = entriesB;
    p->type = POLYGON;

    // TextView com vértices adicionados
    GtkWidget* text_view = gtk_text_view_new();
    gtk_text_view_set_editable (GTK_TEXT_VIEW (text_view), FALSE);
    gtk_text_view_set_cursor_visible (GTK_TEXT_VIEW (text_view), FALSE);
    gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(text_view), GTK_WRAP_CHAR);
    buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (text_view));

    // Botoes
    add_vert_b = gtk_button_new_with_label("Adicionar vértice");
    g_signal_connect_swapped(add_vert_b, "clicked",
        G_CALLBACK (on_add_vert_button), entriesA);
    add_poly_b = gtk_button_new_with_label("Criar");
    g_signal_connect_swapped(add_poly_b, "clicked",
        G_CALLBACK (on_create_poly_button), p);

    grid = gtk_grid_new();
    gtk_grid_set_row_spacing (GTK_GRID(grid), (guint)10);
    gtk_grid_set_column_homogeneous (GTK_GRID(grid), TRUE);
    gtk_container_add(GTK_CONTAINER(window), grid);

    // coloca widgets nos containers
    gtk_grid_attach(GTK_GRID(grid), name_label, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), name_entry, 1, 0, 1, 1); 
    gtk_grid_attach(GTK_GRID(grid), fill_check, 2, 0, 1, 1); 
    gtk_grid_attach(GTK_GRID(grid), t1_label, 0, 1, 2, 1);
    gtk_grid_attach(GTK_GRID(grid), t2_label, 2, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), x_label, 0, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), x_entry, 1, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), y_label, 0, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), y_entry, 1, 3, 1, 1);

    gtk_grid_attach(GTK_GRID(grid), text_view, 2, 2, 1, 2);       
    gtk_grid_attach(GTK_GRID(grid), add_vert_b, 0, 4, 2, 1);
    gtk_grid_attach(GTK_GRID(grid), add_poly_b, 2, 4, 1, 1);

    gtk_widget_show_all(window);
}

// Janela para criar curva e adicionar vértices
void GUI::create_curve_frame() {
    GtkWidget *window, *grid;
    GtkWidget *add_vert_b, *add_poly_b;
    GtkWidget *x_label, *y_label, *t1_label, *t2_label;
    GtkWidget *x_entry, *y_entry, *type_check;
    GtkWidget *name_entry, *name_label;

    // Window
    window = gtk_application_window_new (app);
    gtk_window_set_title (GTK_WINDOW (window), "Adicionar Curva");

    //std::list<Vector2> vert_buffer_list;
    // labels   
    name_label = gtk_label_new("Nome");
    t1_label = gtk_label_new("Adicionar vértice");
    t2_label = gtk_label_new("Vértices");
    x_label = gtk_label_new("Posição x");
    y_label = gtk_label_new("Posição y");
    
    // Entries
    name_entry = gtk_entry_new();
    x_entry = gtk_entry_new();
    y_entry = gtk_entry_new();
    type_check = gtk_check_button_new_with_label("B-Spline?\n (Padrão: Bezier)");

    static GtkWidget *entriesA[2];
    entriesA[0] = x_entry;
    entriesA[1] = y_entry;

    static GtkWidget *entriesB[5];
    entriesB[0] = window;
    entriesB[1] = name_entry;
    entriesB[2] = type_check;
    Params* p = new Params();
    p->entries = entriesB;
    p->type = CURVE;

    // TextView com vértices adicionados
    GtkWidget* text_view = gtk_text_view_new();
    gtk_text_view_set_editable (GTK_TEXT_VIEW (text_view), FALSE);
    gtk_text_view_set_cursor_visible (GTK_TEXT_VIEW (text_view), FALSE);
    gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(text_view), GTK_WRAP_CHAR);
    buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (text_view));

    // Botoes
    add_vert_b = gtk_button_new_with_label("Adicionar vértice");
    g_signal_connect_swapped(add_vert_b, "clicked",
        G_CALLBACK (on_add_vert_button), entriesA);
    add_poly_b = gtk_button_new_with_label("Criar");
    g_signal_connect_swapped(add_poly_b, "clicked",
        G_CALLBACK (on_create_poly_button), p);

    grid = gtk_grid_new();
    gtk_grid_set_row_spacing (GTK_GRID(grid), (guint)10);
    gtk_grid_set_column_homogeneous (GTK_GRID(grid), TRUE);
    gtk_container_add(GTK_CONTAINER(window), grid);

    // coloca widgets nos containers
    gtk_grid_attach(GTK_GRID(grid), name_label, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), name_entry, 1, 0, 1, 1); 
    gtk_grid_attach(GTK_GRID(grid), type_check, 2, 0, 1, 1); 
    gtk_grid_attach(GTK_GRID(grid), t1_label, 0, 1, 2, 1);
    gtk_grid_attach(GTK_GRID(grid), t2_label, 2, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), x_label, 0, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), x_entry, 1, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), y_label, 0, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), y_entry, 1, 3, 1, 1);

    gtk_grid_attach(GTK_GRID(grid), text_view, 2, 2, 1, 2);       
    gtk_grid_attach(GTK_GRID(grid), add_vert_b, 0, 4, 2, 1);
    gtk_grid_attach(GTK_GRID(grid), add_poly_b, 2, 4, 1, 1);

    gtk_widget_show_all(window);
}

/*****************************************
 *
 *  Leitura e escrita de arquivos .obj
 *
 *****************************************/
void GUI::on_import_button(GtkWidget *widget, GtkWidget *window) {
    // Não parece ser o melhor jeito de fazer isso...
    GtkWidget *chooser;
    chooser = gtk_file_chooser_dialog_new(
        "Abrir objeto",
        GTK_WINDOW(widget),
        GTK_FILE_CHOOSER_ACTION_OPEN, 
        ("_Cancel"),  
        GTK_RESPONSE_CANCEL,
        ("_Open"),
        GTK_RESPONSE_ACCEPT,
        NULL);

    gint res = gtk_dialog_run (GTK_DIALOG (chooser));

    if (res == GTK_RESPONSE_ACCEPT) {
        gchar* filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(chooser));
        auto sh = DescOBJ::read_obj(filename);
        for(auto i = 0; i < sh.size(); ++i) {
            Display::add(sh[i]);
            gtk_combo_box_text_append_text (GTK_COMBO_BOX_TEXT (combo), sh[i]->name.c_str());
        }
        //gtk_widget_queue_draw(drawing_area);
    }

    gtk_widget_destroy(chooser);
}

void GUI::on_export_button(GtkWidget *widget, GtkWidget *window) {
    auto selected_id = gtk_combo_box_get_active(GTK_COMBO_BOX(combo));
    auto it = Display::shapes.begin();
    std::advance(it, selected_id);
    std::string path = (*it)->name + ".obj";   

    GtkWidget *chooser;
    chooser = gtk_file_chooser_dialog_new(
        "Salvar objeto",
        GTK_WINDOW(widget),
        GTK_FILE_CHOOSER_ACTION_SAVE, 
        ("_Cancel"),  
        GTK_RESPONSE_CANCEL,
        ("_Save"),
        GTK_RESPONSE_ACCEPT,
        NULL);

    gtk_file_chooser_set_do_overwrite_confirmation(GTK_FILE_CHOOSER(chooser), TRUE);
    gint res = gtk_dialog_run (GTK_DIALOG (chooser));

    if (res == GTK_RESPONSE_ACCEPT) {
        std::string filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(chooser));
        if (filename.find(".")==std::string::npos)
            filename += ".obj";
        DescOBJ::save_obj(filename, *it);
    }
    
    gtk_widget_destroy(chooser);
}

/*****************************************
 *
 *  Janela principal
 *
 *****************************************/
void GUI::activate (GtkApplication* app, gpointer user_data) {
    GUI::app = app;
    GtkWidget* window;
    GtkWidget *grid;
    GtkWidget *up_button, *down_button, *left_button, *right_button;
    GtkWidget *in_button, *out_button;
    GtkWidget *import_button, *export_button;
    GtkWidget *rot_left_button, *rot_right_button;
    GtkWidget *point_button, *line_button, *polygon_button, *curve_button; 
    GtkWidget *move_frame, *create_frame, *add_frame;
    GtkWidget *combo_frame, *transform_frame, *clip_frame;
    GtkWidget *box, *big_box;
    GtkWidget *radio1, *radio2;

    Display::create_all();

    // Janela
    window = gtk_application_window_new (app);
    gtk_window_set_title (GTK_WINDOW (window), "Demo CG");
    gtk_window_set_default_size (GTK_WINDOW(window), 480, 480);

    // Botões de movimentação
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

    // Botões de zoom
    in_button = gtk_button_new_with_label("+");
    g_signal_connect_swapped(in_button, "clicked",
        G_CALLBACK (zoom_in), window);
    out_button = gtk_button_new_with_label("-");
    g_signal_connect_swapped(out_button, "clicked",
        G_CALLBACK (zoom_out), window);

    // Botoes de rotação
    rot_right_button = gtk_button_new_with_label("↷");
    g_signal_connect_swapped(rot_right_button, "clicked",
        G_CALLBACK (rotate_right), window);
    rot_left_button = gtk_button_new_with_label("↶");
    g_signal_connect_swapped(rot_left_button, "clicked",
        G_CALLBACK (rotate_left), window);

    // Grid de botões de movimento
    grid = gtk_grid_new();
    gtk_grid_attach(GTK_GRID(grid), up_button, 1, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), down_button, 1, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), left_button, 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), right_button, 2, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), in_button, 2, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), out_button, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), rot_right_button, 2, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), rot_left_button, 0, 2, 1, 1);
    box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_box_pack_start(GTK_BOX(box), grid, TRUE, TRUE, 0);
    move_frame = gtk_frame_new("Movimentação");
    gtk_container_add(GTK_CONTAINER(move_frame), box);

    // Botoes para criacao de figuras
    Params* p = new Params();
    point_button = gtk_button_new_with_label("Ponto");
    g_signal_connect_swapped(point_button, "clicked",
        G_CALLBACK (create_point_frame), window);
    line_button = gtk_button_new_with_label("Reta");
    g_signal_connect_swapped(line_button, "clicked",
        G_CALLBACK (create_line_frame), window);
    polygon_button = gtk_button_new_with_label("Poligono");
    g_signal_connect_swapped(polygon_button, "clicked",
        G_CALLBACK (create_poly_frame), window);
    curve_button = gtk_button_new_with_label("Curva");
    g_signal_connect_swapped(curve_button, "clicked",
        G_CALLBACK (create_curve_frame), window);
    box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 1);
    gtk_box_pack_start(GTK_BOX(box), point_button, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(box), line_button, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(box), polygon_button, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(box), curve_button, TRUE, TRUE, 0);
    create_frame = gtk_frame_new("Adicionar figura");
    gtk_container_add(GTK_CONTAINER(create_frame), box);

    // Botoes de leitura e escrita de .obj
    import_button = gtk_button_new_with_label("Importar");
    g_signal_connect_swapped(import_button, "clicked",
        G_CALLBACK (on_import_button), window);
    export_button = gtk_button_new_with_label("Exportar");
    g_signal_connect_swapped(export_button, "clicked",
        G_CALLBACK (on_export_button), window);

    box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 1);
    gtk_box_pack_start(GTK_BOX(box), import_button, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(box), export_button, TRUE, TRUE, 0);
    add_frame = gtk_frame_new("Arquivo .obj");
    gtk_container_add(GTK_CONTAINER(add_frame), box);

    // ComboBox (Lista de figuras)
    combo = gtk_combo_box_text_new();
    for (auto it = Display::shapes.begin();
        it != Display::shapes.end(); ++it) {
        const char *textobjeto = (*it)->name.c_str();
        gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT (combo), textobjeto);
    }
    gtk_combo_box_set_active (GTK_COMBO_BOX (combo), 0);
    combo_frame = gtk_frame_new("Selecionar figura");
    gtk_container_add(GTK_CONTAINER(combo_frame), combo);
    
    // ----------GtkNotebook de transformações
    GtkWidget *notebook, *frame, *label;
    notebook = gtk_notebook_new();
    gtk_notebook_set_tab_pos (GTK_NOTEBOOK(notebook), GTK_POS_TOP);
    
    frame = gtk_frame_new("");
    translation_page(frame);
    label = gtk_label_new ("Transladar");
    gtk_notebook_append_page (GTK_NOTEBOOK (notebook), frame, label);
    frame = gtk_frame_new("");
    scaling_page(frame);
    label = gtk_label_new ("Escalonar");
    gtk_notebook_append_page (GTK_NOTEBOOK (notebook), frame, label);
    frame = gtk_frame_new("");
    rotation_page(frame);
    label = gtk_label_new ("Rotacionar");
    gtk_notebook_append_page (GTK_NOTEBOOK (notebook), frame, label);
    // ---------GtkNotebook

    transform_frame = gtk_frame_new("Transformações");
    gtk_container_add(GTK_CONTAINER(transform_frame), notebook);

    // Botões para alterar clipping de linhas
    radio1 = gtk_radio_button_new_with_label(NULL,"Cohen-Sutherland");
    g_signal_connect_swapped(radio1, "pressed",
        G_CALLBACK (set_cohen), window);
    radio2 = gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON (radio1),"Liang_Barsky");
    g_signal_connect_swapped(radio2, "pressed",
        G_CALLBACK (set_liang), window);
    box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 1);
    gtk_box_pack_start(GTK_BOX(box), radio1, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(box), radio2, TRUE, TRUE, 0);
    clip_frame = gtk_frame_new("Algoritmo de clipping de linhas");
    gtk_container_add(GTK_CONTAINER(clip_frame), box);

    // Drawing area
    drawing_area = gtk_drawing_area_new();
    gtk_widget_set_size_request(drawing_area, 
        Window::viewport.x(), Window::viewport.y());
    g_signal_connect(G_OBJECT(drawing_area), "draw",
        G_CALLBACK(draw_cb), nullptr);
    gtk_widget_set_size_request(GTK_WIDGET(drawing_area), 500, 200);

    gtk_frame_set_label_align(GTK_FRAME(move_frame), 0.5, 0.5);
    gtk_frame_set_label_align(GTK_FRAME(create_frame), 0.5, 0.5);
    gtk_frame_set_label_align(GTK_FRAME(add_frame), 0.5, 0.5);
    gtk_frame_set_label_align(GTK_FRAME(combo_frame), 0.5, 0.5);
    gtk_frame_set_label_align(GTK_FRAME(transform_frame), 0.5, 0.5);
    gtk_frame_set_label_align(GTK_FRAME(clip_frame), 0.5, 0.5);

    box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 1);
    gtk_box_pack_start(GTK_BOX(box), move_frame, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(box), create_frame, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(box), add_frame, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(box), combo_frame, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(box), transform_frame, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(box), clip_frame, TRUE, TRUE, 5);

    big_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_box_pack_start(GTK_BOX(big_box), drawing_area, TRUE, TRUE, 2);
    gtk_box_pack_start(GTK_BOX(big_box), box, FALSE, TRUE, 0);
    
    gtk_container_add(GTK_CONTAINER(window), big_box);
    gtk_widget_show_all(window);
}

// Devia ter feito isso com glade :thinking:
