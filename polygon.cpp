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
    if (name == "Fsquare") {
        draw_fill2();
        return;
    }
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

    // Evitar bad_alloc
    if (clipVerts.empty()) return;

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

void to_String (std::list<Vector2> l) {
    for (auto i : l) {
        std::cout << std::string(i) << std::endl;
    }
    if (l.empty())
        std::cout << "empty" << std::endl;
}


void Polygon::draw_fill2() {
    auto edges = Window::edges();
    std::list<Vector2> output;
    for (auto it = verts.begin(); it != verts.end(); ++it) {
        output.push_back(Window::world_to_norm(*it));
    }
    for (auto edge : edges) {
        if (output.empty()) {
            break;
        }
        auto input = std::list<Vector2>();
        input.swap(output);
        Vector2 p1 = input.back();
        for (auto p2 : input) {
            //if (p2 == Vector2(0,0)) continue;
            if (&p1 == &p2) continue;
            AB line(p1, p2);
            if(is_inside(p2, edge)) {
                if (!is_inside(p1, edge)) {
                    //std::cout << std::string(p1) << " p2" << std::string(p2) << std::endl;
                    //std::cout << "clip a " << std::string(Window::clip_line(line).a) << std::endl;
                    line = Window::clip_line(line);
                    if (!line.empty) {
                        output.push_back(line.a);
                    }
                }
                output.push_back(p2);
            } else if (is_inside(p1, edge)) {
                line = Window::clip_line(line);
                if (!line.empty) {
                    output.push_back(line.b);
                }
            }
            p1 = p2;
        }
    }
    if (output.empty()) {
        return;
    }

    //to_String(output);
    //std::cout << "---" << std::endl;

    auto it = output.begin();
    auto pos = *it;
    for (; it != output.end(); ++it) {
        //std::cout << "pos " << std::string(Window::norm_to_vp(pos)) << std::endl;
        //std::cout << "it " << std::string(Window::norm_to_vp(*it)) << std::endl;
        AB edge (pos, *it);
        Window::draw_pline(edge);
        pos = *it;
    }    
    
    //AB edge2 (output.back(), output.front());
    //Window::draw_pline(edge2);        
    cairo_fill(Window::cr);
    cairo_close_path(Window::cr);
    
}

bool Polygon::is_inside(Vector2 coord, AB edge) {
    return (edge.b.x() - edge.a.x()) * (coord.y() - edge.a.y()) < 
           (edge.b.y() - edge.a.y()) * (coord.x() - edge.a.x());
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

    
