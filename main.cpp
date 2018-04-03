#include "gui.h"

/**
 *	Ativa funções de interface do GTK no namespace GUI.
 */
static void activateGUI (GtkApplication* app, gpointer user_data) {
    GUI::activate(app, user_data);
}

static gboolean frame (void* data) {
    Window::animate();
    gtk_widget_queue_draw(GUI::drawing_area);
    return TRUE;
}
int main (int argc, char **argv) {
    /*Window::update_boundaries();
    Window::clip_line(AB(Vector2(-0.3, 0.5), Vector2(0.3, 1.1)));
    return 0;*/
    GtkApplication *app;
    int status;

    unsigned int fps = 60;
    float time = 1000 / (float) fps;
    g_timeout_add(time, frame, NULL);
    app = gtk_application_new ("org.gtk.cg", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK(activateGUI), NULL);
    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);


    return status;
}
