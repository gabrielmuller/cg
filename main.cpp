#include "gui.h"

/**
 *	Ativa funções de interface do GTK no namespace GUI.
 */
static void activateGUI (GtkApplication* app, gpointer user_data) {
    GUI::activate(app, user_data);
}

static gboolean test (void* data) {
    Window::animate();
    gtk_widget_queue_draw(GUI::drawing_area);
    return TRUE;
}
int main (int argc, char **argv) {
    /*
    AB teste (Vector2(-10, -10), Vector2(-10, 20));
    AB outro (Vector2(3, 3), Vector2(-2, -1));
    std::cout << "UM: " << Window::clip_line(teste).empty << std::endl; 
    std::cout << "ZERO: " << Window::clip_line(outro).empty << std::endl;
    return 0;
    */
    GtkApplication *app;
    int status;

    unsigned int fps = 60;
    float time = 1000 / (float) fps;
    g_timeout_add(time, test, NULL);
    app = gtk_application_new ("org.gtk.cg", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK(activateGUI), NULL);
    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);


    return status;
}
