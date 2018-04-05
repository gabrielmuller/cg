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
    if (fill && !open) {
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
            AB line(p1, p2);
            if(Window::is_inside(p2, edge)) {
                if (!Window::is_inside(p1, edge)) {
                    line = Window::clip_to_edge(edge, line);
                    output.push_back(line.a);
                }
                output.push_back(p2);
            } else if (Window::is_inside(p1, edge)) {
                line = Window::clip_to_edge(edge, line);
                output.push_back(line.b);
            }
            p1 = p2;
        }
    }
    if (output.empty()) return;
    auto it = output.begin();
    auto pos = *it;
    for (; it != output.end(); ++it) {
        AB edge (pos, *it);
        Window::draw_pline(edge);
        pos = *it;
    }           
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

    
