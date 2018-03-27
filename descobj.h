#ifndef DESCOBJ_H
#define DESCOBJ_H

#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include "shape.h"
#include "point.h"
#include "line.h"
#include "polygon.h"
#include "vector2.h"

/**
 *  Descritor para arquivos .obj
 *  Lê/escreve uma figura/objeto em um arquivo, de modo que
 *  cada arquivo representa um objeto, ex:
 *  figura.obj:
 *  "
 *  v 1 1
 *  v -1 -1
 *  "
 *  Representa uma linha de (1,1) a (-1, -1)
 */
namespace DescOBJ {
    Shape* read_obj(const std::string &path);
    void save_obj(const std::string &path, Shape *sh);
    const std::vector<std::string> split(const std::string &s, char dl);
}

#endif