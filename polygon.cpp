#include "polygon.h"
#include <iostream>

Polygon::Polygon (std::string name, std::list<Vector2> verts) : 
    open(false),
    fill(false),
    verts(verts),
    Shape::Shape(name) {}

Polygon::Polygon (std::string name) :
    open(false),
    fill(false),
    Shape::Shape(name) {}

Polygon::Polygon (std::string name, bool fill) :
    open(false),
    fill(fill),
    Shape::Shape(name) {}

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
    std::list<Vector2> clipVerts;

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

    it = clipVerts.begin();
    pos = *it;
    for (; it != clipVerts.end(); ++it) {
        //std::cout << std::string(Window::norm_to_vp(*it)) << std::endl;
        AB edge (pos, *it);
        Window::draw_pline(edge);
        pos = *it;
    }    
    
    AB edge2 (clipVerts.back(), clipVerts.front());
    Window::draw_pline(edge2);        
    cairo_fill(Window::cr);
    cairo_close_path(Window::cr);
}

void Polygon::transform(const Transformation& t) {
    for (auto it = verts.begin(); it != verts.end(); ++it) {
        t.transform(*it);
    }
}

/**
 * @brief       Retorna o centro geométrico do polígono.
 */
Vector2 Polygon::center() const {
    float dx = 0;
    float dy = 0;
    for (auto it = verts.begin(); it != verts.end(); ++it) {
        dx += it->x();
        dy += it->y();
    } 

    // pra não dar problema quando o primeiro é igual
    // ao último vértice do polígono
    // deus me perdoe
    float size = verts.size();
    if (verts.back().x() == verts.front().x() &&
    verts.back().y() == verts.front().y() &&
    size > 1) {
        dx -= verts.front().x();
        dy -= verts.front().y();
        size--;
    }
    
    dx /= size;
    dy /= size;

    return Vector2(dx, dy);
}

    
