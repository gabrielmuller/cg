#ifndef SHAPE_H
#define SHAPE_H

#include <list>
#include <string>
#include "vector2.h"
#include "window.h"

struct Shape {
    std::string name;

    Shape (std::string name); 

    /**
     * @brief       Desenha o polígono no viewport. 
     */
    virtual void draw () = 0;

    /**
     * @brief       Realiza uma transformação qualquer na forma. 
     *
     * @param[in]   t           transformação
     */
    virtual void transform (const Transformation& t) = 0;

    void translate (const Vector2& distance);
    void scale (const Vector2& amount);
    void rotate (float rad, const Vector2& center);
    void rotate (float rad);
    virtual Vector2 center () const = 0;
};

#endif
