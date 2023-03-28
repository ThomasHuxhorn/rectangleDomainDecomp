#ifndef READMESH_HPP
#define READMESH_HPP

#include <fstream>

#include "Mesh.hpp"


Mesh readMeshXFN(const std::string& fileName) {
    std::cout << "Read " << fileName << "\n";
    std::ifstream f(fileName);
    if(not f) throw std::system_error(errno, std::system_category(), "failed to open " + fileName + " ");

    std::string line;
    int nRandknoten = 0;
    int nGebietsknoten = 0;
    int nElemente = 0;

    std::getline(f, line);
    std::getline(f, line);
    f >> nRandknoten >> std::ws;
    std::getline(f, line);
    f >> nGebietsknoten >> std::ws;

    for(int i=0; i < 7; ++i) {
        std::getline(f, line);
    }

    Mesh mesh;
    mesh.nodes.resize(nRandknoten + nGebietsknoten);

    for(size_t i=0; i < mesh.nodes.size(); ++i) {
        size_t IP;
        f >> IP >> mesh.nodes[IP].x >> mesh.nodes[IP].y >> mesh.nodes[IP].z >> std::ws;
    }

    std::getline(f, line);
    std::getline(f, line);
    f >> nElemente >> std::ws;
    std::getline(f, line);
    std::getline(f, line);
    std::getline(f, line);

    mesh.elements.resize(nElemente);

    for(size_t IE=0; IE < mesh.elements.size(); ++IE) {
        int tmp;
        f >> mesh.elements[IE][0] >> mesh.elements[IE][1] >> mesh.elements[IE][2] >> tmp >> tmp >> std::ws;
    }

    return mesh;
}



#endif // READMESH_HPP
