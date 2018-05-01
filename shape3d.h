#ifndef SHAPE3D_H
#define SHAPE3D_H

#include <vector>
#include "edge3d.h"
#include "rotation.h"

struct Shape3D {
    std::string name;

    Shape3D (std::string name); 

    virtual void draw () = 0;

    /**
     * @brief       Realiza uma transformação qualquer na forma. 
     *
     * @param[in]   t           transformação
     */
    virtual void transform (const Transformation& t) = 0;

    void translate (Vector3 distance);
    void scale (Vector3 amount);
    void rotate (Rotation rot);
    virtual Vector3 center () const = 0;
};

#endif
