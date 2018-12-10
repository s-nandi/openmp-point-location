#ifndef FACE_H
#define FACE_H

#include <vector>

struct pt;
struct vertex;
struct halfedge;

struct face
{
    halfedge *startEdge = nullptr;
    int index;

    face(){}
    face(int index) : index(index) {}

    std::vector <halfedge*> getEdges() const;
    std::vector <vertex*> getVertices() const;
    std::vector <pt> getPoints() const;
    std::vector <face*> getNeighbors() const;
};

#endif
