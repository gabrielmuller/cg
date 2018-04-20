#include "descobj.h"

std::vector<Drawable*> DescOBJ::read_obj(const std::string &path) {
    std::ifstream file(path);
    std::string l;
    std::vector<std::string> line;
    const std::string filename = split(path, '/').back();
    //const std::string name = split(filename, '.').front();
    
    // Info sobre as figuras
    std::vector<Vector2> verts, shape_verts;
    std::vector<Drawable*> dbs;
    std::vector<int> index;
    std::string name;

    try {
        while (std::getline(file, l)) {
            line = split(l, ' ');
            if (line.size() == 0) 
            {
                continue; 
            } 
            else if (line[0] == "v") // Vertice
            {   
                std::vector<float> point;
                /*
                  inicio line[1] (primeira coordenada), 
                  fim line.end()-1 (não pega 3a coordenada (2D)),
                  vector onde será inserido,
                  função de transformação
                  calculo lambda wooow
                */
                // TODO mudar pra 3D
                std::transform(
                    line.begin()+1, 
                    line.end()-1,
                    std::back_inserter(point),
                    [](const std::string &str) -> float { 
                        return std::stof(str); });
                verts.push_back(Vector2(point[0], point[1]));
            } 
            else if (line[0] == "o") // Objeto
            {   
                name = line[1];
                std::getline(file, l);
                auto line2 = split(l, ' ');
                if (line2[0] == "p") // Ponto
                {   
                    auto i = std::stoi(line2[1]) -1;
                    Point* p = new Point(name, verts[i].x(), verts[i].y());
                    dbs.push_back(p);
                    index.clear();
                }
                else if (line2[0] == "l" || line2[0] == "f") // Linha, polígono
                {   
                    std::transform(
                        line2.begin()+1, 
                        line2.end(),
                        std::back_inserter(index),
                        [](const std::string &str) -> int { 
                            return std::stoi(str) -1; });
                    std::transform(
                            index.begin(), 
                            index.end(),
                            std::back_inserter(shape_verts),
                            [verts](int i) -> Vector2 { 
                                return verts[i]; });
                    if (shape_verts.size() == 2) {
                        Line* p = new Line(name, shape_verts);
                        dbs.push_back(p);
                    } else {   
                        auto fill = false;
                        if (line2[0] == "f") fill = true;
                        Polygon* p = new Polygon(name, shape_verts, fill);
                        dbs.push_back(p);
                    }
                    index.clear();
                    shape_verts.clear();
                }
            }
        }
    } catch (const std::invalid_argument &ia) {
        // tratar exceção
    }

    // TODO modificar pra funcionar com qualquer forma
    if (dbs.empty()) {
        Drawable* sh;
        name = split(filename, '.').front();
        if (verts.size() == 1)
            sh = new Point(name, verts.front().x(), verts.front().y());
        else if (verts.size() == 2)
            sh = new Line(name, verts);
        else
            sh = new Polygon(name, verts);
        dbs.push_back(sh);
    }

    return dbs;
}

void DescOBJ::save_obj(const std::string &path, Drawable *sh) {
    std::ofstream file(path);
    // TODO modificar pra funcionar com qualquer forma
    if (dynamic_cast<Vertices*>(sh) == nullptr) {
        Point *a = dynamic_cast<Point*>(sh);
        file << "v " << a->position.x() << " " << a->position.y() << " 0" << std::endl;
    } else {
        Vertices *a = dynamic_cast<Vertices*>(sh);
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
