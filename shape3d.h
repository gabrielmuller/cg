#ifndef SHAPE3D_H
#define SHAPE3D_H

#include <vector>
#include "edge3d.h"
#include "drawable.h"

struct Shape3D : Drawable {

    Shape3D (std::string name); 

    /**
     * @brief       Realiza uma transformação qualquer na forma. 
     *
     * @param[in]   t           transformação
     */
    virtual void transform (Transformation t) = 0;

    void translate (Vector3 distance);
    void scale (Vector3 amount);
    void rotate (Vector3 angles, Vector3 center);
    void rotate (Vector3 angles);
    virtual Vector3 center () const = 0;
};

#endif
