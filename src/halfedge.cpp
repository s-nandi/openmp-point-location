#include "halfedge.h"
#include "face.h"
#include "vertex.h"
#include "pt.h"

vertex* halfedge::destination() const
{
    return twin ? twin -> origin : next -> origin;
}

void halfedge::setIncidentFace(face* &f)
{
    incidentFace = f;
    f -> startEdge = this;
}

void halfedge::setOrigin(vertex* &o)
{
    origin = o;
    o -> incidentEdge = this;
}

void halfedge::setNext(halfedge* &n)
{
    next = n;
    n -> prev = this;
}

void halfedge::setTwin(halfedge* &t)
{
    twin = t;
    t -> twin = this;
}

std::pair <pt, pt> halfedge::getEndpoints() const
{
    return {origin -> point, destination() -> point};
}
