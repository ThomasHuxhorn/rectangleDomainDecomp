
#ifndef WRITEMESH_HPP
#define WRITEMESH_HPP

#include <fstream>

#include "Mesh.hpp"

void writeMeshXFN(const Mesh& mesh) {
    std::string fileName = "decomp.dat";
    std::cout << "Write " << fileName << "\n";

    std::ofstream f(fileName);
    if(not f) throw std::system_error(errno, std::system_category(), "failed to open " + fileName + " ");

    f << "C system.dat, erzeugt von rectangleDomainDecond\n";
    f << "C Anzahl der Randknoten:\n";
    f << 0 << "\n";
    f << "C Anzahl der Gebietsknoten:\n";
    f << mesh.nodes.size() << "\n";
    f << "C Koordinaten und Skalarwerte der Knoten\n";
    f << "C --------------------------------------\n";
    f << "C Zuerst die Randknoten  (Anzahl s.o.),\n";
    f << "C dann die Gebietsknoten (Anzahl s.o.).\n";
    f << "C ------------+-------------+-------------+---------------\n";
    f << "C     Nr.     |  x-Koord.   |   y-Koord.  |   Skalarwert\n";
    f << "C ------------+-------------+-------------+---------------\n";

    for(size_t IP=0; IP < mesh.nodes.size(); ++IP) {
        f << IP << " " << mesh.nodes[IP].x << " " << mesh.nodes[IP].y << " " << mesh.nodes[IP].z << "\n";
    }

    f << "C ------------------------------------------------------------\n";
    f << "C Anzahl der Elemente:\n";
    f << mesh.elements.size() << "\n";
    f << "C Elementverzeichnis\n";
    f << "C ------------------------------------------------------------\n";
    f << "C    Knoten i  Knoten j  Knoten k   Kennung     Nr.\n";

    for(size_t IE=0; IE < mesh.elements.size(); ++IE) {
        f << mesh.elements[IE][0] << " " << mesh.elements[IE][1] << " " << mesh.elements[IE][2] << " 0 " << IE << "\n";
    }

    f << "C ------------------------------------------------------------\n";

}

#endif // WRITEMESH_HPP
