#include "polygon.h"
#include <iostream>

Polygon::Polygon (std::string name, std::vector<Vector2> verts) :
    open(false),
    fill(false),
    Vertices::Vertices(name, verts) {}

Polygon::Polygon (std::string name, std::vector<Vector2> verts, bool fill) : 
    open(false),
    fill(fill),
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
    if (fill && !open) {
        draw_fill();
        return;
    }
    auto it = verts.begin();
    Vector2 pos = *it;

    for (; it != verts.end(); ++it) {
        Edge edge (pos, *it);
        Window::draw_line(edge);
        pos = *it;
    }
    if (!open) {
        Edge edge (verts.back(), verts.front());
        Window::draw_line(edge);
    }
}

/**
 * @brief   Desenha polígonos preenchidos,
 *          aplicando clipping Cohen-Sutherland.
 *          Utiliza Window::clip_to_edge()
 */
void Polygon::draw_fill() {
    auto edges = Window::edges();
    std::list<Vector2> output;
    for (auto it = verts.begin(); it != verts.end(); ++it) {
        output.push_back(Window::world_to_norm(*it));
    }
    for (auto edge : edges) {
        if (output.empty()) break;
        auto input = std::list<Vector2>();
        input.swap(output);
        Vector2 p1 = input.back();
        for (auto p2 : input) {
            if (&p1 == &p2) continue;
            Edge line(p1, p2);
            if(Window::is_inside(p2, edge)) {
                if (!Window::is_inside(p1, edge)) {
                    auto v = Window::clip_to_edge(edge, line);
                    output.push_back(v);
                }
                output.push_back(p2);
            } else if (Window::is_inside(p1, edge)) {
                auto v = Window::clip_to_edge(edge, line);
                output.push_back(v);
            }
            p1 = p2;
        }
    }
    if (output.empty()) return;
    auto it = output.begin();
    auto pos = *it;
    for (; it != output.end(); ++it) {
        Edge edge (pos, *it);
        Window::draw_pline(edge);
        pos = *it;
    }           
    cairo_fill(Window::cr);
    cairo_close_path(Window::cr); 
}
