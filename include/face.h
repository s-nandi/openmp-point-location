#ifndef FACE_H
#define FACE_H

#include <vector>
#include "pt.h"

struct vertex;
struct halfedge;

struct face
{
    halfedge *startEdge = nullptr;
    int index;
    pt centroid;

    face(){}
    face(int index) : index(index) {}

    std::vector <halfedge*> getEdges() const;
    std::vector <vertex*> getVertices() const;
    std::vector <pt> getPoints() const;
    std::vector <face*> getNeighbors() const;
    
    void initCentroid();
};

#endif
