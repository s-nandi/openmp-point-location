#ifndef VERTEX_H
#define VERTEX_H

#include <vector>
#include "pt.h"

struct halfedge;

struct vertex
{
    pt point;
    halfedge* incidentEdge = nullptr;
    int index;

    vertex(){}
    vertex(pt &point, int index) : point(point), index(index) {}

    std::vector <halfedge*> getEdges();
};

#endif
