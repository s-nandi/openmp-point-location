#include "dcel.h"
#include "vertex.h"
#include "halfedge.h"
#include "face.h"

face* DCEL::sequential_get_closest_face(const pt &point)
{
    ptT min_distance = pt::distance2(samplefaces[0] -> centroid, point);
    face* closest = samplefaces[0];
    for (int i = 1; i < numSample; i++)
    {
	auto possible_distance = pt::distance2(samplefaces[i] -> centroid, point);
	if (possible_distance < min_distance)
	{
	    min_distance = possible_distance;
	    closest = samplefaces[i];
	}
    }
    return closest;
}

int DCEL::sequential_locate(const pt &point)
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

void DCEL::sequential_locations(const std::vector <pt> &points, std::vector <int> &out)
{
    int n = points.size();
    for (int i = 0; i < n; i++)
	out[i] = sequential_locate(points[i]);
}
