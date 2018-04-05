#ifndef VERTICES_H
#define VERTICES_H
#include <vector>
#include "vector2.h"
#include "shape.h"

/* Vertices é um shape composto por um vetor
  * de vértices que tem como transformação
  * a transformação de cada um de seus vértices.
  *
  * Usado para aproveitar código em comum entre
  * polígonos e curvas.
  */

struct Vertices : Shape {
    std::vector<Vector2> verts;

    Vertices (std::string name, std::vector<Vector2> verts); 
    Vertices (std::string name); 

    void transform (const Transformation& t) override;
    Vector2 center() const override;
};

#endif
