#include "face.h"
#include "halfedge.h"
#include "pt.h"
#include "vertex.h"

std::vector <halfedge*> face::getEdges() const
{
    auto firstEdge = startEdge, currEdge = firstEdge;
    std::vector <halfedge*> res;
    do
    {
	res.push_back(currEdge);
	currEdge = currEdge -> next;
    }
    while (currEdge != firstEdge);
    return res;
}

std::vector <vertex*> face::getVertices() const
{
    auto edges = getEdges();
    std::vector <vertex*> res;
    for (halfedge* &edge: edges)
    {
	res.push_back(edge -> origin);
    }
    return res;
}

std::vector <pt> face::getPoints() const
{
    auto edges = getEdges();
    std::vector <pt> res;
    for (halfedge* &edge: edges)
    { 
	res.push_back(edge -> origin -> point);
    }
    return res;
}

std::vector <face*> face::getNeighbors() const
{
    auto edges = getEdges();
    std::vector <face*> res;
    for (halfedge* &edge: edges)
    {
	if (edge -> twin)
	{
	    res.push_back(edge -> twin -> incidentFace);
	}
    }
    return res;
}

pt face::getCentroid() const
{
    auto points = getPoints();
    pt centroid = {0, 0};
    for (pt &p: points)
    {
	centroid = centroid + p;
    }
    centroid = centroid / points.size();
    return centroid;
}

std::ostream & operator << (std::ostream &os, face &f)
{
    os << "Face " << f.index << ": ";
    auto points = f.getPoints();
    for (pt &point: points)
        os << point << " | ";
    return os;
}

