#ifndef HALFEDGE_DEFINED
#define HALFEDGE_DEFINED

#include <utility>

struct pt;
struct vertex;
struct face;

struct halfedge
{
    vertex *origin = nullptr;
    halfedge *twin = nullptr, *next = nullptr, *prev = nullptr;
    face *incidentFace = nullptr;

    vertex* destination() const;
    void setIncidentFace(face* &f);
    void setOrigin(vertex* &o);
    void setNext(halfedge* &n);
    void setTwin(halfedge* &t);
    std::pair <pt, pt> getEndpoints() const;
};

#endif
