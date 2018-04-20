#ifndef DISPLAY_H
#define DISPLAY_h

#include <list>
#include "shape.h"

// display file
namespace Display {
    extern std::list<Drawable*> dbs;
    void create_all ();
    void add(Drawable* sh);
    void draw_all ();
}

#endif
