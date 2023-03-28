

#ifndef MESH_HPP
#define MESH_HPP

/*
 * A simple mesh class for demo purpose.
 */

#include <vector>
#include <array>

struct Point3D {
    double x = 0;
    double y = 0;
    double z = 0;
};

struct Element {
    std::array<int, 3> IDs;

    // Implementation of the [] operator.
    // Shortcut so we can write mesh.elements[IP][1]
    int operator[](int i) const {
        return IDs[i];
    }

    // non const overload
    int& operator[](int i){
        return IDs[i];
    }
};

struct Mesh {
    std::vector<Point3D> nodes;
    std::vector<Element> elements;
};

#endif // MESH_HPP
