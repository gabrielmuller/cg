#include "gui.h"
#include "vector2.h"
#include <iostream>

/**
 *	Ativa funções de interface do GTK no namespace GUI.
 */
static void activateGUI (GtkApplication* app, gpointer user_data) {
    GUI::activate(app, user_data);
}

int main (int argc, char **argv) {
    /* Teste Matriz */
    /*Matrix a (2, 2);
    Matrix b (2, 2);
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            a[i][j] = i + j + 1;
            b[i][j] = i * j - 3;
        }
    }
    std::cout << (std::string) c << " = " << (std::string) a << " * " << (std::string) b << std::endl;
    Matrix c = a * b; */
    

    Vector2 u (3, 4);
    Vector2 v (-1, -3);
    Vector2 w;
    w = u * v;
    std::cout << (std::string) w << std::endl;

    return 0;
    //std::cout << (std::string) (Vector2(0.4, 0.3) - Vector2(3, 3));
    // inicia app
    
    /*
    
    GtkApplication *app;
    int status;

    app = gtk_application_new ("org.gtk.cg", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK(activateGUI), NULL);
    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status; */
}
