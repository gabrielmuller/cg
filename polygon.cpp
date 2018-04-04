#include "polygon.h"
#include <iostream>

Polygon::Polygon (std::string name, std::vector<Vector2> verts) : 
    open(false),
    fill(false),
    Vertices::Vertices(name, verts) {}

Polygon::Polygon (std::string name) :
    open(false),
    fill(false),
    Vertices::Vertices(name) {}

Polygon::Polygon (std::string name, bool fill) :
    open(false),
    fill(fill),
    Vertices::Vertices(name) {}

void Polygon::draw () {
    if (verts.empty()) {
        return;
    }
    if (fill) {
        draw_fill();
        return;
    }
    auto it = verts.begin();
    Vector2 pos = *it;

    for (; it != verts.end(); ++it) {
        AB edge (pos, *it);
        Window::draw_line(edge);
        pos = *it;
    }
    if (!open) {
        AB edge (verts.back(), verts.front());
        Window::draw_line(edge);
    }
}

// deus me perdoe
void Polygon::draw_fill() {
    auto it = verts.begin();
    std::vector<Vector2> clipVerts;

    Vector2 pos = *it;

    for (; it != verts.end(); ++it) {
        AB edge (pos, *it);
        edge.a = Window::world_to_norm(edge.a);
        edge.b = Window::world_to_norm(edge.b);
        edge = Window::clip_line(edge);
        if (!edge.empty) {
            clipVerts.push_back(edge.a);
            clipVerts.push_back(edge.b);
        }
        pos = *it;
    }

    // close poly
    AB edge (verts.back(), verts.front());
    edge.a = Window::world_to_norm(edge.a);
    edge.b = Window::world_to_norm(edge.b);
    edge = Window::clip_line(edge);
    if (!edge.empty) {
        clipVerts.push_back(edge.a);
        clipVerts.push_back(edge.b);
    }
    // close poly

    // Evitar bad_alloc
    if (clipVerts.empty()) return;
    //std::cout << "size " << clipVerts.size() << std::endl;

    it = clipVerts.begin();
    pos = *it;
    //std::cout << "---" << std::endl;
    for (; it != clipVerts.end(); ++it) {
        //std::cout << "pos " << std::string(Window::norm_to_vp(pos)) << std::endl;
        //std::cout << "it " << std::string(Window::norm_to_vp(*it)) << std::endl;
        AB edge (pos, *it);
        Window::draw_pline(edge);
        pos = *it;
    }    
    
    AB edge2 (clipVerts.back(), clipVerts.front());
    Window::draw_pline(edge2);        
    cairo_fill(Window::cr);
    cairo_close_path(Window::cr);
}
