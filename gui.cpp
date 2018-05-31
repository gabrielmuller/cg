#include "gui.h"
#include <iostream>

GtkWidget* GUI::drawing_area;
GtkApplication* GUI::app;
GtkWidget* GUI::combo;
GtkTextBuffer* GUI::buffer;
GtkTextIter GUI::iter;

//--- Local
std::vector<Vector2> vert_buffer_list;
std::vector<Edge3D> edge_buffer_list;

enum {
    POLYGON,
    CURVE
};

enum Axis { X=0, Y=1, Z=2 };
Axis axis = X;
Axis shape_axis = X;
//--- Local

// desenha no drawing area
gboolean GUI::draw_cb(GtkWidget *widget, cairo_t* cr, gpointer* data) {
    // fundo preto
    cairo_set_source_rgb(cr, 0, 0, 0);
    cairo_paint(cr);

    cairo_set_line_width(cr, 1);

    Window::cr = cr;
    Window::draw_borders();
    // linha branca
    cairo_set_source_rgb(cr, 1, 1, 1);

    // ?

    Display::draw_all();
    return FALSE;
}

void GUI::set_cohen () {
    Window::clipping_algorithm = COHEN_SUTHERLAND;
}

void GUI::set_liang () {
    Window::clipping_algorithm = LIANG_BARSKY;
}

void GUI::set_2D () {
    Window::render = ONLY_2D;
}

void GUI::set_cavalier () {
    Window::render = CAVALIER;
}

void GUI::set_cam_perspective () {
    Window::render = PERSPECTIVE;
}

void GUI::set_rotation_axis_x() { axis = X; }
void GUI::set_rotation_axis_y() { axis = Y; }
void GUI::set_rotation_axis_z() { axis = Z; }
void GUI::set_shape_axis_x() { shape_axis = X; }
void GUI::set_shape_axis_y() { shape_axis = Y; }
void GUI::set_shape_axis_z() { shape_axis = Z; }

void GUI::move (Vector2 amount) {
    if (Window::render == ONLY_2D) {
        Transformation rot = 
            Transformation::rotation(-Window::goal.angle, Vector2(0, 0));
        amount = (Transformation) amount * rot;
        Window::goal.position = Window::goal.position + amount;
    } else {
        /*
        Transformation rot =
            Transformation::rotation3D(Rotation(
            Window::real3.position, Window::real3.forward, 
            Vector3(amount.x(), amount.y(), 0)));
            */
        //amount = (Transformation) amount * rot;
        Window::real3.position =
            Window::real3.position + Vector3(amount.x(), amount.y(), 0);
    }
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
    GUI::rotate_cam(true);
}

void GUI::rotate_left() {
    GUI::rotate_cam(false);
}

void GUI::rotate_cam(const bool right) {
    float amount = right ? 0.1 : -0.1;
    if (Window::render != ONLY_2D) {
        Vector3 rot_axis;
        rot_axis.matrix[0][axis] = 1;
        Window::rotate(rot_axis, amount);
    } else {
        Window::goal.angle += amount;
    }
}

void GUI::set_perspective(GtkAdjustment *perspective_scale) {
    auto value = gtk_adjustment_get_value ((perspective_scale));
    Window::real3.dist_pp = value;
    Window::real3.position = Vector3(Window::real3.position.x(),
    Window::real3.position.y(),
    -value);
}

/*****************************************
 *
 *   "Páginas" ou frames 
 *   e callbacks de transformação
 *
 *****************************************/

void GUI::on_translate_button(GtkWidget **entry, GtkWidget *widget) {
    std::string coord_x = gtk_entry_get_text (GTK_ENTRY(entry[0]));
    std::string coord_y = gtk_entry_get_text (GTK_ENTRY(entry[1]));
    std::string coord_z = gtk_entry_get_text (GTK_ENTRY(entry[2]));

    if (coord_x == "") coord_x = "0";
    if (coord_y == "") coord_y = "0";
    if (coord_z == "") coord_z = "0";

    // TODO: achar a figura por um modo mais rápido
    auto shape_name = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(combo));
    if (Display::find_shape(shape_name) != nullptr) {
        auto shape = Display::find_shape(shape_name);
        shape->translate(Vector2(std::stof(coord_x), std::stof(coord_y)));
    } else {
        auto shape = Display::find_shape3D(shape_name);
        shape->translate(Vector3(std::stof(coord_x), std::stof(coord_y), std::stof(coord_z)));
    }
    g_free(shape_name);
    //gtk_widget_queue_draw(drawing_area);
}

// TODO: conseguir juntar translation_page e scaling_page
void GUI::translation_page (GtkWidget* frame) {
    GtkWidget *grid;
    GtkWidget *translate_button;
    GtkWidget *x_label, *y_label, *z_label, *n_label;
    GtkWidget *x_entry, *y_entry, *z_entry;

    // labels
    x_label = gtk_label_new("Coordenada x");
    y_label = gtk_label_new("Coordenada y");
    z_label = gtk_label_new("Coordenada z");
    n_label = gtk_label_new("Vetor de translação");

    // Entries
    x_entry = gtk_entry_new();
    y_entry = gtk_entry_new();
    z_entry = gtk_entry_new();

    static GtkWidget *entries[3];
    entries[0] = x_entry;
    entries[1] = y_entry;
    entries[2] = z_entry;

    // Botoes
    translate_button = gtk_button_new_with_label("Ok");
    g_signal_connect_swapped(translate_button, "clicked",
        G_CALLBACK (on_translate_button), entries);

    grid = gtk_grid_new();

    // (grid, widget, coluna, linha, tamanhox, tamanhoy)
    gtk_grid_attach(GTK_GRID(grid), n_label, 0, 0, 2, 1);
    gtk_grid_attach(GTK_GRID(grid), x_label, 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), x_entry, 1, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), y_label, 0, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), y_entry, 1, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), z_label, 0, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), z_entry, 1, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), translate_button, 0, 4, 2, 1);

    gtk_container_add(GTK_CONTAINER(frame), grid);
}

void GUI::on_scale_button(GtkWidget **entry, GtkWidget *widget) {
    std::string coord_x = gtk_entry_get_text (GTK_ENTRY(entry[0]));
    std::string coord_y = gtk_entry_get_text (GTK_ENTRY(entry[1]));
    std::string coord_z = gtk_entry_get_text (GTK_ENTRY(entry[2]));

    if (coord_x == "") coord_x = "0";
    if (coord_y == "") coord_y = "0";
    if (coord_z == "") coord_z = "0";

    auto shape_name = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(combo));
    if (Display::find_shape(shape_name) != nullptr) {
        auto shape = Display::find_shape(shape_name);
        shape->scale(Vector2(std::stof(coord_x), std::stof(coord_y)));
    } else {
        auto shape = Display::find_shape3D(shape_name);
        shape->scale(Vector3(std::stof(coord_x), std::stof(coord_y), std::stof(coord_z)));
    }
    g_free(shape_name);
    //gtk_widget_queue_draw(drawing_area);
}

void GUI::scaling_page(GtkWidget* frame) {
    GtkWidget *grid;
    GtkWidget *scale_button;
    GtkWidget *x_label, *y_label, *z_label, *n_label;
    GtkWidget *x_entry, *y_entry, *z_entry;

    // labels
    x_label = gtk_label_new("Sx");
    y_label = gtk_label_new("Sy");
    z_label = gtk_label_new("Sz");
    n_label = gtk_label_new("Fatores de escala");

    // Entries
    x_entry = gtk_entry_new();
    y_entry = gtk_entry_new();
    z_entry = gtk_entry_new();

    static GtkWidget *entries[3];
    entries[0] = x_entry;
    entries[1] = y_entry;
    entries[2] = z_entry;

    // Botoes
    scale_button = gtk_button_new_with_label("Ok");
    g_signal_connect_swapped(scale_button, "clicked",
        G_CALLBACK (on_scale_button), entries);

    grid = gtk_grid_new();

    // (grid, widget, coluna, linha, tamanhox, tamanhoy)
    gtk_grid_attach(GTK_GRID(grid), n_label, 0, 0, 2, 1);
    gtk_grid_attach(GTK_GRID(grid), x_label, 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), x_entry, 1, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), y_label, 0, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), y_entry, 1, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), z_label, 0, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), z_entry, 1, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), scale_button, 0, 4, 3, 1);

    gtk_container_add(GTK_CONTAINER(frame), grid);
}

void GUI::on_rotate_button(GtkWidget **entry, GtkWidget *widget) {
    auto graus = gtk_entry_get_text (GTK_ENTRY(entry[0]));
    std::string coord_x = gtk_entry_get_text (GTK_ENTRY(entry[1]));
    std::string coord_y = gtk_entry_get_text (GTK_ENTRY(entry[2]));
    std::string coord_z = gtk_entry_get_text (GTK_ENTRY(entry[3]));
    bool center = false;

    /* Se os campos x e y estiverem em branco, a rotação é
     * em relação ao centro geométrico
     */
    if (coord_x == "") {
        coord_x = "0";
        coord_y = "0";
        coord_z = "0";
        center = true;
    }

    float rad = std::stof(graus) * (M_PI/180);

    auto shape_name = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(combo));
    auto shape = Display::find_shape(shape_name);
    if (shape != nullptr) {
        if (center)
            shape->rotate(rad);
        else
            shape->rotate(rad, Vector2(std::stof(coord_x), std::stof(coord_y)));
    } else {
        //beautiful
        auto shape = Display::find_shape3D(shape_name);
        Vector3 axis_vector;
        if (shape_axis == X) 
            axis_vector = Vector3(1,0,0);
        else if (shape_axis == Y) 
            axis_vector = Vector3(0,1,0);
        else 
            axis_vector = Vector3(0,0,1);
        if(center)
            shape->rotate(Rotation(axis_vector, rad));
        else
            shape->rotate(Rotation( 
                Vector3(std::stof(coord_x), std::stof(coord_y), std::stof(coord_z)), 
                axis_vector, rad));
    }
    g_free(shape_name);
    //gtk_widget_queue_draw(drawing_area);
}

void GUI::rotation_page (GtkWidget* frame) {
    GtkWidget *grid, *rotate_button, *d_label, *d_entry;
    GtkWidget *x_label, *y_label, *x_entry, *y_entry, *z_label, *z_entry;
    GtkWidget *radio_x, *radio_y, *radio_z;

    radio_x = gtk_radio_button_new_with_label(NULL,"x");
    g_signal_connect_swapped(radio_x, "pressed",
        G_CALLBACK (set_shape_axis_x), NULL);
    radio_y = gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON (radio_x),"y");
    g_signal_connect_swapped(radio_y, "pressed",
        G_CALLBACK (set_shape_axis_y), NULL);
    radio_z = gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON (radio_x),"z");
    g_signal_connect_swapped(radio_z, "pressed",
        G_CALLBACK (set_shape_axis_z), NULL);

    // labels
    d_label = gtk_label_new("Graus");
    x_label = gtk_label_new("Coordenada x");
    y_label = gtk_label_new("Coordenada y");
    z_label = gtk_label_new("Coordenada z");

    // Entries
    d_entry = gtk_entry_new();
    x_entry = gtk_entry_new();
    y_entry = gtk_entry_new();
    z_entry = gtk_entry_new();

    static GtkWidget *entries[4];
    entries[0] = d_entry;
    entries[1] = x_entry;
    entries[2] = y_entry;
    entries[3] = z_entry;

    // Botoes
    rotate_button = gtk_button_new_with_label("Ok");
    g_signal_connect_swapped(rotate_button, "clicked",
        G_CALLBACK (on_rotate_button), entries);

    gtk_widget_set_tooltip_text (GTK_WIDGET(rotate_button),
        "Para rotação em relação ao centro do\n objeto, deixe os campos x,y,z em\n branco.");
    grid = gtk_grid_new();

    // (grid, widget, coluna, linha, tamanhox, tamanhoy)
    gtk_grid_attach(GTK_GRID(grid), d_label, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), d_entry, 1, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), x_label, 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), x_entry, 1, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), y_label, 0, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), y_entry, 1, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), z_label, 0, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), z_entry, 1, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), radio_x, 0, 4, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), radio_y, 1, 4, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), radio_z, 2, 4, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), rotate_button, 0, 5, 2, 1);

    gtk_container_add(GTK_CONTAINER(frame), grid);
}

/*****************************************
 *
 *  Frame e callbacks para criar figura 2D
 *  ou curva (de Bezier ou B-Spline)
 *
 *****************************************/

// Termina criação de figura2D/curva
void GUI::on_finish_2D_button(Params* p, GtkWidget *widget) {
    std::string name = gtk_entry_get_text (GTK_ENTRY(p->entries[1]));
    Shape *sh;
    bool option = gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(p->entries[2]));
    if (p->type == POLYGON) {
        if (vert_buffer_list.size() == 1)
            sh = new Point(name, vert_buffer_list.front().x(),
                vert_buffer_list.front().y());
        else if (vert_buffer_list.size() == 2)
            sh = new Line(name, vert_buffer_list);
        else if (vert_buffer_list.size() >= 3)
            sh = new Polygon(name, vert_buffer_list, option);
    } else {
        if (option) {
            sh = new Spline(name, vert_buffer_list);
        } else {
            sh = new Bezier(name, vert_buffer_list);
        }
    }

    // limpar buffer para próximo shape
    vert_buffer_list.clear();

    Display::add(sh);
    //gtk_widget_queue_draw(drawing_area);
    gtk_combo_box_text_append_text (GTK_COMBO_BOX_TEXT (combo), name.c_str());
    gtk_widget_destroy(GTK_WIDGET(p->entries[0]));
}

// Adiciona vértice ao vetor/buffer de vértices
void GUI::on_add_vert_button(GtkWidget **entry, GtkWidget *widget) {
    auto coord_x = gtk_entry_get_text (GTK_ENTRY(entry[0]));
    auto coord_y = gtk_entry_get_text (GTK_ENTRY(entry[1]));
    vert_buffer_list.push_back(Vector2(std::stof(coord_x), std::stof(coord_y)));
    // Simplificar isso depois
    std::ostringstream s;
    s << "(" << coord_x << ", " << coord_y << "), ";
    std::string ss = s.str(); //
    gtk_text_buffer_get_end_iter(buffer, &iter);
    gtk_text_buffer_insert(buffer, &iter, ss.c_str(), -1);
}

void GUI::on_create_2D_button () {
    create_vertices_frame(true);
}

void GUI::on_create_curve_button () {
    create_vertices_frame(false);
}

// Janela para criar figura2D/curva
void GUI::create_vertices_frame(bool isPoly) {
    std::string name = isPoly ? "Figura 2D" : "Curva";
    name = "Adicionar " + name;
    std::string option = isPoly ? "Preencher? (Polígono)" : "B-Spline? (senão Bézier)";

    GtkWidget *window, *grid;
    GtkWidget *add_vert_button, *finish_2D_button;
    GtkWidget *x_label, *y_label, *t1_label, *t2_label;
    GtkWidget *x_entry, *y_entry;
    GtkWidget *name_entry, *name_label, *option_check;

    // Window
    window = gtk_application_window_new (app);
    gtk_window_set_title (GTK_WINDOW (window), name.c_str());
    gtk_window_set_default_size (GTK_WINDOW(window), 200, 200);

    //std::list<Vector2> vert_buffer_list;
    // labels   
    name_label = gtk_label_new("Nome");
    t1_label = gtk_label_new("Adicionar vértice");
    t2_label = gtk_label_new("Vértices");
    x_label = gtk_label_new("x");
    y_label = gtk_label_new("y");
    
    // Entries
    name_entry = gtk_entry_new();
    option_check = gtk_check_button_new_with_label(option.c_str());
    x_entry = gtk_entry_new();
    y_entry = gtk_entry_new();

    gtk_entry_set_width_chars(GTK_ENTRY(name_entry), 5);
    gtk_entry_set_width_chars(GTK_ENTRY(x_entry), 5);
    gtk_entry_set_width_chars(GTK_ENTRY(y_entry), 5);

    static GtkWidget *entriesA[2];
    entriesA[0] = x_entry;
    entriesA[1] = y_entry;

    static GtkWidget *entriesB[5];
    entriesB[0] = window;
    entriesB[1] = name_entry;
    entriesB[2] = option_check;
    Params* p = new Params();
    p->entries = entriesB;
    p->type = isPoly ? POLYGON : CURVE;

    // TextView com vértices adicionados
    GtkWidget* text_view = gtk_text_view_new();
    GtkWidget* scrolledwindow = gtk_scrolled_window_new(NULL, NULL);
    gtk_text_view_set_editable (GTK_TEXT_VIEW (text_view), FALSE);
    gtk_text_view_set_cursor_visible (GTK_TEXT_VIEW (text_view), FALSE);
    gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(text_view), GTK_WRAP_CHAR);
    buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (text_view));
    gtk_widget_set_size_request(scrolledwindow, 100,20);
    gtk_container_add(GTK_CONTAINER(scrolledwindow), text_view);

    // Botoes
    add_vert_button = gtk_button_new_with_label("Adicionar vértice");
    g_signal_connect_swapped(add_vert_button, "clicked",
        G_CALLBACK (on_add_vert_button), entriesA);
    finish_2D_button = gtk_button_new_with_label("Criar");
    g_signal_connect_swapped(finish_2D_button, "clicked",
        G_CALLBACK (on_finish_2D_button), p);

    grid = gtk_grid_new();
    gtk_grid_set_column_spacing (GTK_GRID(grid), (guint)1);
    gtk_grid_set_row_spacing (GTK_GRID(grid), (guint)10);
    gtk_container_add(GTK_CONTAINER(window), grid);

    gtk_grid_attach(GTK_GRID(grid), name_label, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), name_entry, 1, 0, 1, 1); 
    gtk_grid_attach(GTK_GRID(grid), option_check, 2, 0, 1, 1); 
    gtk_grid_attach(GTK_GRID(grid), t1_label, 0, 1, 2, 1);
    gtk_grid_attach(GTK_GRID(grid), t2_label, 2, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), x_label, 0, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), x_entry, 1, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), y_label, 0, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), y_entry, 1, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), scrolledwindow, 2, 2, 1, 2);       
    gtk_grid_attach(GTK_GRID(grid), add_vert_button, 0, 4, 2, 1);
    gtk_grid_attach(GTK_GRID(grid), finish_2D_button, 2, 4, 1, 1);

    gtk_widget_show_all(window);
}

/*****************************************
 *
 *  Janela e callbacks pra criar poliedro 
 *
 *****************************************/

// Adiciona poliedro
void GUI::on_finish_3D_button(GtkWidget **entry, GtkWidget *widget) {
    std::string nome = gtk_entry_get_text (GTK_ENTRY(entry[1]));
    Polyhedron* sh = new Polyhedron (nome, edge_buffer_list);
    Display::add(sh);
    //gtk_widget_queue_draw(drawing_area);
    gtk_combo_box_text_append_text (GTK_COMBO_BOX_TEXT (combo), nome.c_str());
    gtk_widget_destroy(GTK_WIDGET(entry[0]));
}

// Adiciona aresta
void GUI::on_add_edge_button(GtkWidget **entry, GtkWidget *widget) {
    auto x1 = gtk_entry_get_text (GTK_ENTRY(entry[0]));
    auto y1 = gtk_entry_get_text (GTK_ENTRY(entry[1]));
    auto z1 = gtk_entry_get_text (GTK_ENTRY(entry[2]));
    auto x2 = gtk_entry_get_text (GTK_ENTRY(entry[3]));
    auto y2 = gtk_entry_get_text (GTK_ENTRY(entry[4]));
    auto z2 = gtk_entry_get_text (GTK_ENTRY(entry[5]));
    Vector3 a (std::stof(x1), std::stof(y1),std::stof(z1));
    Vector3 b (std::stof(x2), std::stof(y2),std::stof(z2));
    edge_buffer_list.push_back(Edge3D(a, b));
    // Simplificar isso depois
    std::ostringstream s;
    s << "[(" << x1 << ", " << y1 << ", " << z1 << "), " << "(" << x2 << ", " << y2 << ", " << z2 << ")]," << '\n'  ;
    std::string ss = s.str(); //
    gtk_text_buffer_get_end_iter(buffer, &iter);
    gtk_text_buffer_insert(buffer, &iter, ss.c_str(), -1);
}

void GUI::on_create_3D_button () {
    GtkWidget *window, *grid;       //janela e grid
    GtkWidget *add_edge_button, *finish_3D_button;     //botoes
    GtkWidget *x_label, *y_label, *z_label;  //labels
    GtkWidget *p1_label, *p2_label, *name_label, *texte_view_label;
    GtkWidget *x1_entry, *x2_entry, *y1_entry, *y2_entry,
              *z1_entry, *z2_entry; // entries
    GtkWidget *name_entry;

    // Window
    window = gtk_application_window_new (app);
    gtk_window_set_title (GTK_WINDOW (window), "Adicionar Figura 3D");
   
    // labels
    name_label = gtk_label_new("Nome");
    x_label = gtk_label_new("x");
    y_label = gtk_label_new("y");
    z_label = gtk_label_new("z");
    p1_label = gtk_label_new("Ponto a");
    p2_label = gtk_label_new("Ponto b");
    texte_view_label = gtk_label_new("Arestas");

    // Entries
    name_entry = gtk_entry_new();
    x1_entry = gtk_entry_new();
    y1_entry = gtk_entry_new();
    z1_entry = gtk_entry_new();
    x2_entry = gtk_entry_new();
    y2_entry = gtk_entry_new();
    z2_entry = gtk_entry_new();

    static GtkWidget *entriesA[6];
    entriesA[0] = x1_entry;
    entriesA[1] = y1_entry;
    entriesA[2] = z1_entry;
    entriesA[3] = x2_entry;
    entriesA[4] = y2_entry;
    entriesA[5] = z2_entry;

    gtk_entry_set_width_chars(GTK_ENTRY(name_entry), 5);
    for (auto entry : entriesA)
        gtk_entry_set_width_chars(GTK_ENTRY(entry), 5);

    static GtkWidget *entriesB[2];
    entriesB[0] = window;
    entriesB[1] = name_entry;

    // TextView com arestas adicionadas
    GtkWidget* text_view = gtk_text_view_new();
    GtkWidget* scrolledwindow = gtk_scrolled_window_new(NULL, NULL);
    gtk_text_view_set_editable (GTK_TEXT_VIEW (text_view), FALSE);
    gtk_text_view_set_cursor_visible (GTK_TEXT_VIEW (text_view), FALSE);
    gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(text_view), GTK_WRAP_CHAR);
    buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (text_view));
    gtk_widget_set_size_request(scrolledwindow, 200,20);
    gtk_container_add(GTK_CONTAINER(scrolledwindow), text_view);

    // Botoes
    add_edge_button = gtk_button_new_with_label("Adicionar aresta");
    g_signal_connect_swapped(add_edge_button, "clicked",
        G_CALLBACK (on_add_edge_button), entriesA);
    finish_3D_button = gtk_button_new_with_label("Criar");
    g_signal_connect_swapped(finish_3D_button, "clicked",
        G_CALLBACK (on_finish_3D_button), entriesB);

    grid = gtk_grid_new();
    gtk_grid_set_column_spacing (GTK_GRID(grid), (guint)1);
    gtk_grid_set_row_spacing (GTK_GRID(grid), (guint)10);
    gtk_container_add(GTK_CONTAINER(window), grid);

    // (grid, widget, coluna, linha, tamanhox, tamanhoy)
    gtk_grid_attach(GTK_GRID(grid), name_label, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), name_entry, 1, 0, 2, 1);
    gtk_grid_attach(GTK_GRID(grid), p1_label, 0, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), p2_label, 0, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), x_label, 1, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), x1_entry, 1, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), x2_entry, 1, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), y_label, 2, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), y1_entry, 2, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), y2_entry, 2, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), z_label, 3, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), z1_entry, 3, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), z2_entry, 3, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), texte_view_label, 4, 0, 2, 1);
    gtk_grid_attach(GTK_GRID(grid), scrolledwindow, 4, 1, 2, 4);
    gtk_grid_attach(GTK_GRID(grid), add_edge_button, 0, 4, 2, 1);
    gtk_grid_attach(GTK_GRID(grid), finish_3D_button, 2, 4, 2, 1);

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
    GtkWidget *box, *big_box;

    Display::create_all();

    // Janela
    window = gtk_application_window_new (app);
    gtk_window_set_title (GTK_WINDOW (window), "Demo CG");
    gtk_window_set_default_size (GTK_WINDOW(window), 480, 480);

    // Botões de movimentação
    GtkWidget *up_button, *down_button, *left_button, *right_button; 
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
    GtkWidget *in_button, *out_button;
    in_button = gtk_button_new_with_label("+");
    g_signal_connect_swapped(in_button, "clicked",
        G_CALLBACK (zoom_in), window);
    out_button = gtk_button_new_with_label("-");
    g_signal_connect_swapped(out_button, "clicked",
        G_CALLBACK (zoom_out), window);

    // Botoes de rotação
    GtkWidget *rot_left_button, *rot_right_button;
    rot_right_button = gtk_button_new_with_label("↷");
    g_signal_connect_swapped(rot_right_button, "clicked",
        G_CALLBACK (rotate_right), window);
    rot_left_button = gtk_button_new_with_label("↶");
    g_signal_connect_swapped(rot_left_button, "clicked",
        G_CALLBACK (rotate_left), window);

    // Escolher eixo de rotação (x,y,z)
    GtkWidget *radio_x, *radio_y, *radio_z;
    radio_x = gtk_radio_button_new_with_label(NULL,"x");
    g_signal_connect_swapped(radio_x, "pressed",
        G_CALLBACK (set_rotation_axis_x), window);
    radio_y = gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON (radio_x),"y");
    g_signal_connect_swapped(radio_y, "pressed",
        G_CALLBACK (set_rotation_axis_y), window);
    radio_z = gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON (radio_x),"z");
    g_signal_connect_swapped(radio_z, "pressed",
        G_CALLBACK (set_rotation_axis_z), window);

    // Grid de botões de movimento
    GtkWidget *move_frame;
    grid = gtk_grid_new();
    gtk_grid_attach(GTK_GRID(grid), up_button, 1, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), down_button, 1, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), left_button, 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), right_button, 2, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), in_button, 2, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), out_button, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), rot_right_button, 2, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), rot_left_button, 0, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), radio_x, 0, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), radio_y, 1, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), radio_z, 2, 3, 1, 1);
    box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_box_pack_start(GTK_BOX(box), grid, TRUE, TRUE, 0);
    move_frame = gtk_frame_new("Movimentação");
    gtk_container_add(GTK_CONTAINER(move_frame), box);

    // Botoes para criacao de figuras
    GtkWidget *create_2D_button, *create_curve_button, *create_3D_button;
    GtkWidget *create_frame;
    Params* p = new Params();
    create_2D_button = gtk_button_new_with_label("2D");
    g_signal_connect_swapped(create_2D_button, "clicked",
        G_CALLBACK (on_create_2D_button), window);
    create_curve_button = gtk_button_new_with_label("Curva");
    g_signal_connect_swapped(create_curve_button, "clicked",
        G_CALLBACK (on_create_curve_button), window);
    create_3D_button = gtk_button_new_with_label("3D");
    g_signal_connect_swapped(create_3D_button, "clicked",
        G_CALLBACK (on_create_3D_button), window);
    box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 1);
    gtk_box_pack_start(GTK_BOX(box), create_2D_button, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(box), create_curve_button, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(box), create_3D_button, TRUE, TRUE, 0);
    create_frame = gtk_frame_new("Adicionar figura");
    gtk_container_add(GTK_CONTAINER(create_frame), box);

    // Botoes de leitura e escrita de .obj
    GtkWidget *import_button, *export_button, *add_frame;
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

    // ----------GtkComboBox (Lista de figuras)
    GtkWidget *combo_frame;
    combo = gtk_combo_box_text_new();
    for (auto it = Display::shapes.begin(); it != Display::shapes.end(); ++it) {
        const char *sh_name = (*it)->name.c_str();
        gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT (combo), sh_name);
    }
    for (auto it = Display::shapes3D.begin(); it != Display::shapes3D.end(); ++it) {
        const char *sh_name = (*it)->name.c_str();
        gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT (combo), sh_name);
    }
    gtk_combo_box_set_active (GTK_COMBO_BOX (combo), 0);
    combo_frame = gtk_frame_new("Selecionar figura");
    gtk_container_add(GTK_CONTAINER(combo_frame), combo);
    // ----------GtkComboBox
    
    // ----------GtkNotebook de transformações
    GtkWidget *notebook, *frame, *label, *transform_frame;
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
    transform_frame = gtk_frame_new("Transformações");
    gtk_container_add(GTK_CONTAINER(transform_frame), notebook);
    // ---------GtkNotebook

    // ---------GtkRadioButton (escolher clipping)
    GtkWidget *radio1, *radio2, *clip_frame;
    radio1 = gtk_radio_button_new_with_label(NULL,"Cohen-Sutherland");
    g_signal_connect_swapped(radio1, "pressed",
        G_CALLBACK (set_cohen), window);
    radio2 = gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON (radio1),"Liang-Barsky");
    g_signal_connect_swapped(radio2, "pressed",
        G_CALLBACK (set_liang), window);
    box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 1);
    gtk_box_pack_start(GTK_BOX(box), radio1, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(box), radio2, TRUE, TRUE, 0);
    clip_frame = gtk_frame_new("Algoritmo de clipping de linhas");
    gtk_container_add(GTK_CONTAINER(clip_frame), box);
    // ---------GtkRadioButton
    
    // ---------GtkRadioButton (escolher entre 2D / 3D)
    /*
    GtkWidget *render1, *render2, *render3, *render_frame;
    render1 = gtk_radio_button_new_with_label(NULL,"2D");
    g_signal_connect_swapped(render1, "pressed",
        G_CALLBACK (set_2D), window);
    render2 = gtk_radio_button_new_with_label_from_widget(
        GTK_RADIO_BUTTON (render1),"Cavaleira");
    g_signal_connect_swapped(render2, "pressed",
        G_CALLBACK (set_cavalier), window);
    g_signal_connect_swapped(render3, "pressed",
        G_CALLBACK (set_cam_perspective), window);
    box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 1);
    gtk_box_pack_start(GTK_BOX(box), render1, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(box), render2, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(box), render3, TRUE, TRUE, 0);
    render_frame = gtk_frame_new("Modo de renderização");
    gtk_container_add(GTK_CONTAINER(render_frame), box);
    */
    // ---------GtkRadioButton

    // --------- GtkScale (perspectiva)
    GtkWidget *perspective_scale;
    GtkAdjustment *adj;
    // value, lower, upper, step_increment, page_increment, page_size
    adj = gtk_adjustment_new (5.0, 2.0, 15.0, 1.0, 1.0, 0.0);
    perspective_scale = gtk_scale_new (GTK_ORIENTATION_HORIZONTAL, GTK_ADJUSTMENT(adj));
    g_signal_connect_swapped (adj, "value_changed",
        G_CALLBACK (set_perspective), adj);

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
    gtk_box_pack_start(GTK_BOX(box), perspective_scale, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(box), clip_frame, TRUE, TRUE, 5);

    big_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_box_pack_start(GTK_BOX(big_box), drawing_area, TRUE, TRUE, 2);
    gtk_box_pack_start(GTK_BOX(big_box), box, FALSE, TRUE, 0);
    
    gtk_container_add(GTK_CONTAINER(window), big_box);
    gtk_widget_show_all(window);
}

// Devia ter feito isso com glade
/*
 
⠀⠰⡿⠿⠛⠛⠻⠿⣷
⠀⠀⠀⠀⠀⠀⣀⣄⡀⠀⠀⠀⠀⢀⣀⣀⣤⣄⣀⡀
⠀⠀⠀⠀⠀⢸⣿⣿⣷⠀⠀⠀⠀⠛⠛⣿⣿⣿⡛⠿⠷
⠀⠀⠀⠀⠀⠘⠿⠿⠋⠀⠀⠀⠀⠀⠀⣿⣿⣿⠇
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠉⠁

⠀⠀⠀⠀⣿⣷⣄⠀⢶⣶⣷⣶⣶⣤⣀
⠀⠀⠀⠀⣿⣿⣿⠀⠀⠀⠀⠀⠈⠙⠻⠗
⠀⠀⠀⣰⣿⣿⣿⠀⠀⠀⠀⢀⣀⣠⣤⣴⣶⡄
⠀⣠⣾⣿⣿⣿⣥⣶⣶⣿⣿⣿⣿⣿⠿⠿⠛⠃
⢰⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡄
⢸⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡁
⠈⢿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠁
⠀⠀⠛⢿⣿⣿⣿⣿⣿⣿⡿⠟
⠀⠀⠀⠀⠀⠉⠉⠉ 

*/
