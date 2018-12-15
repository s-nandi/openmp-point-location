#include "dcel.h"
#include "vertex.h"
#include "halfedge.h"
#include "face.h"
#include <limits>

int DCEL::hybrid_locate(const pt &point)
{
    face* curr_face = sequential_get_closest_face(point);
    while (true)
    {
	face* next_face = nullptr;
	std::pair <pt, pt> ray = {curr_face -> centroid, point};
	auto edges = curr_face -> getEdges();
	for (halfedge* e: edges)
	{
	    auto endpoints = e -> getEndpoints();
	    if (pt::intersects(ray.first, ray.second, endpoints.first, endpoints.second))
	    {
		next_face = e -> twin -> incidentFace;
	    }
	}
	if (!next_face)
	    break;
	curr_face = next_face;
	if (curr_face -> index < 0)
	    break;
    }
    return curr_face -> index;
}

void DCEL::hybrid_locations(const std::vector <pt> &points, std::vector <int> &out)
{
    int n = points.size();
    #pragma omp parallel
    #pragma omp single
    for (int i = 0; i < n; i++)
    {
	#pragma omp task
	out[i] = hybrid_locate(points[i]);
    }
}
