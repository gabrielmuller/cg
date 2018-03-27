#include "descobj.h"

Shape* DescOBJ::read_obj(const std::string &path) {
    std::ifstream file(path);
    std::string l;
    std::vector<std::string> line;
    const std::string filename = split(path, '/').back();
    const std::string name = split(filename, '.').front();
    std::list<Vector2> verts;

    try {
        while (std::getline(file, l)) {
            line = split(l, ' ');
            if (line.size() == 0) {
                continue; 
            } else if (line[0] == "v") { // Vertice
                std::vector<float> point;
                /*inicio line[1] (primeira coordenada), 
                  fim line.end()-1 (não pega 3a coordenada (2D)),
                  vector onde será inserido,
                  função de transformação
                */
                // TODO mudar pra 3D
                std::transform(
                    line.begin()+1, 
                    line.end()-1,
                    std::back_inserter(point),
                    [](const std::string &str) -> float { return std::stof(str); });
                verts.push_back(Vector2(point[0], point[1]));
            }
        }
    } catch (const std::invalid_argument &ia) {
        // tratar exceção
    }

    // TODO modificar pra funcionar com qualquer forma
    Shape* sh;
    if (verts.size() == 1) {
        sh = new Point(name, verts.front().x(), verts.front().y());
    } else if (verts.size() == 2) {
        sh = new Line(name, verts);
    } else {
        // TODO retirar gambiarra porque Polygon não se fecha sozinho
        verts.push_back(verts.front());
        sh = new Polygon(name, verts);
    }

    return sh;
}

void DescOBJ::save_obj(const std::string &path, Shape *sh) {
    std::ofstream file(path);
    // TODO modificar pra funcionar com qualquer forma
    if (dynamic_cast<Polygon*>(sh) == nullptr) {
        Point *a = dynamic_cast<Point*>(sh);
        file << "v " << a->position.x() << " " << a->position.y() << " 0" << std::endl;
    } else {
        Polygon *a = dynamic_cast<Polygon*>(sh);
        for (auto it = a->verts.begin(); it != a->verts.end(); ++it) {
            file << "v " << it->x() << " " << it->y() << " 0" << std::endl;
        }
    }
}

const std::vector<std::string> DescOBJ::split(const std::string &s, char c) {
    std::string buff {""};
    std::vector<std::string> v;
    for (auto n:s) {
        if (n!= c) buff+=n; else
        if (n == c && buff != "") {v.push_back(buff); buff="";}
    }
    if (buff != "") v.push_back(buff);
    return v;
}