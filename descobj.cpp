#include "descobj.h"
#include <locale.h>

/**
 *  Importa figuras 3D
 */
std::vector<Shape3D*> DescOBJ::read_obj(const std::string &path) {
    setlocale(LC_ALL, "C"); 
    std::ifstream file(path);
    std::string l;
    std::vector<std::string> line;
    const std::string filename = split(path, '/').back();
    //const std::string name = split(filename, '.').front();
    
    // Info sobre as figuras
    std::vector<Vector3> verts;
    std::vector<Vector3> shape_verts;
    std::vector<Edge3D> shape_edges;
    std::vector<Shape3D*> shapes;
    std::vector<int> index;
    std::string name; 

    try {
        while (std::getline(file, l)) {
            line = split(l, ' ');
            if (line.size() == 0) 
            {
                continue; 
            } 
            else if (line[0] == "v") // Vertice 3D
            {   
                std::vector<float> point;
                /*
                  inicio line[1] (primeira coordenada), 
                  fim line.end()
                  vector onde será inserido,
                  função de transformação
                  calculo lambda wooow
                */
                std::transform(
                    line.begin()+1, 
                    line.end(),
                    std::back_inserter(point),
                    [](const std::string &str) -> float { 
                        return std::stof(str); });
                verts.push_back(Vector3(point[0], point[1], point[2]));
            } 
            else if (line[0] == "o") // Objeto
            {   
                name = line[1];
                //std::getline(file, l);
                //auto line2 = split(l, ' ');
                // Criar figura 3D
                Polyhedron* p = new Polyhedron(name);
                shapes.push_back(p);
                
            }
            else if (line[0] == "p") // TODO Ponto (3D!!) 
            {   
                /*auto i = std::stoi(line2[1]) -1;
                Point* p = new Point(name, vector3_to_2(verts[i]).x(), vector3_to_2(verts[i]).y());
                shapes.push_back(p);*/
                index.clear();
            }
            else if (line[0] == "l") // Linha, polígono (3D!!)
            {   
                std::transform(
                    line.begin()+1, 
                    line.end(),
                    std::back_inserter(index),
                    [](const std::string &str) -> int { 
                        return std::stoi(str) -1; });
                std::transform(
                        index.begin(), 
                        index.end(),
                        std::back_inserter(shape_verts),
                        [verts](int i) -> Vector3 { 
                            return (verts[i]); });
                for(auto i = 0; i < shape_verts.size()-1; ++i) {
                    shape_edges.push_back(Edge3D(shape_verts[i], shape_verts[i+1]));
                }
                // Adicionar arestas a linha ou polígono 3D
                Polyhedron* lastShape = dynamic_cast<Polyhedron*>(shapes.back());
                lastShape->edges = shape_edges;
                // Limpar vetores
                index.clear();
                shape_verts.clear();
                shape_edges.clear();
            }
            else if (line[0] == "f") // Face de poliedro (3D)
            {
                std::transform(
                    line.begin()+1, 
                    line.end(),
                    std::back_inserter(index),
                    [](const std::string &str) -> int { 
                        return std::stoi(str) -1; });
                std::transform(
                        index.begin(), 
                        index.end(),
                        std::back_inserter(shape_verts),
                        [verts](int i) -> Vector3 { 
                            return (verts[i]); });
                // Adicionar arestas a figura 3D
                Polyhedron* lastShape = dynamic_cast<Polyhedron*>(shapes.back());
                for(auto i = 0; i < shape_verts.size(); ++i) {
                    if(i == shape_verts.size()-1) {
                        if(lastShape != nullptr)
                            lastShape->edges.push_back(Edge3D(shape_verts[i], shape_verts[0]));
                        else
                            shape_edges.push_back(Edge3D(shape_verts[i], shape_verts[0]));
                    } else {
                        if(lastShape != nullptr)
                            lastShape->edges.push_back(Edge3D(shape_verts[i], shape_verts[i+1]));
                        else
                            shape_edges.push_back(Edge3D(shape_verts[i], shape_verts[0]));
                    }
                }
                // Limpar vetores
                index.clear();
                shape_verts.clear();
            }
        }
    } catch (const std::invalid_argument &ia) {
        // TODO tratar exceção
    }

    // TODO Ponto 3D
    if (shapes.empty()) {
        name = split(filename, '.').front();
        Polyhedron* p = new Polyhedron(name, shape_edges);   
        shapes.push_back(p);
    }
    return shapes;
}

// TODO modificar pra salvar qualquer forma
void DescOBJ::save_obj(const std::string &path, Shape *sh) {
    std::ofstream file(path);
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
