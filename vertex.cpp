#include "pt.h"
#include "vertex.h"
#include "halfedge.h"

std::vector <halfedge*> vertex::getEdges()
{
    auto firstEdge = incidentEdge, currEdge = incidentEdge;
    std::vector <halfedge*> res;
    do
    {
	res.push_back(currEdge);
	if (!currEdge -> twin) break;

	currEdge = currEdge -> twin -> next;
    }
    while (currEdge != firstEdge);
    return res;
}

std::ostream & operator << (std::ostream &os, vertex &v)
{
    os << "Vertex: " << v.point << " -> ";
    auto edges = v.getEdges();
    for (auto &edge: edges)
    {
        os << (edge -> endpoint() -> point) << " ";
    }
    return os;
}
