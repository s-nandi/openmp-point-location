#ifndef HALFEDGE_DEFINED
#define HALFEDGE_DEFINED

struct vertex;
struct face;

struct halfedge
{
    vertex *origin;
    halfedge *twin = nullptr, *next, *prev;
    face *incidentFace;

    vertex* destination();
    void setIncidentFace(face* &f);
    void setOrigin(vertex* &o);
    void setNext(halfedge* &n);
    void setTwin(halfedge* &t);
};

#endif
