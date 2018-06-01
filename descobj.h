#ifndef DESCOBJ_H
#define DESCOBJ_H

#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>

#include "shape.h"
#include "point.h"
#include "line.h"
#include "polygon.h"
#include "vector2.h"
#include "vector3.h"
#include "edge3d.h"
#include "polyhedron.h"

/**
 *  Descritor para arquivos .obj
 *  Lê/escreve figuras de um arquivo.obj
 */
namespace DescOBJ {
    std::vector<Shape3D*> read_obj(const std::string &path);
    void save_obj(const std::string &path, Shape *sh);
    const std::vector<std::string> split(const std::string &s, char dl);
}

#endif
