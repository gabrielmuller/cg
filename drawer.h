#ifndef DRAWER_H
#define DRAWER_h

#include "point.h"
#include "polygon.h"
#include <gtk/gtk.h>

namespace Drawer {
    void draw (cairo_t* cr, Point point);
    void draw (cairo_t* cr, Polygon poly);
}


#endif
