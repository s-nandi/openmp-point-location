#ifndef HALFEDGE_DEFINED
#define HALFEDGE_DEFINED

#include <utility>

struct pt;
struct vertex;
struct face;

struct halfedge
{
    vertex *origin;
    halfedge *twin = nullptr, *next, *prev;
    face *incidentFace;

    vertex* destination() const;
    void setIncidentFace(face* &f);
    void setOrigin(vertex* &o);
    void setNext(halfedge* &n);
    void setTwin(halfedge* &t);
    std::pair <pt, pt> getEndpoints() const;
};

#endif
