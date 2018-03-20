#include "gui.h"
#include <iostream>
#include "vector2.h"

/**
 *	Ativa funções de interface do GTK no namespace GUI.
 */
static void activateGUI (GtkApplication* app, gpointer user_data) {
    GUI::activate(app, user_data);
}

int main (int argc, char **argv) {
    /* Teste Matriz
    Matrix a = Matrix(2, 2);
    Matrix b = Matrix(2, 2);
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            a[i][j] = i + j + 1;
            b[i][j] = i * j - 3;
        }
    }
    Matrix c = Matrix::multiply(a, b);
    std::cout << (std::string) c;
    // inicia app
    Transformation* v = new Vector2 (4, 5);
    Transformation* t = Transformation::rotation(3.1415, Vector2(1, 1));
    t->transform(v);
    std::cout << (std::string) *v;
    return 0; */

    
    GtkApplication *app;
    int status;

    app = gtk_application_new ("org.gtk.cg", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK(activateGUI), NULL);
    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;
}
