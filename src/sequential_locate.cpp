#include "dcel.h"
#include "vertex.h"
#include "halfedge.h"
#include "face.h"
#include <utility>

const ptT INF = 1e20;

distance_index_pair seq_pair_min(distance_index_pair a, distance_index_pair b)
{
    return (a.first < b.first) ? a : b;
}

face* DCEL::sequential_get_closest_face(pt &point)
{
    distance_index_pair pairs[MAX_SAMPLE_SIZE];
    for (int i = 0; i < numSample; i++)
	pairs[i] = {pt::distance2(samplefaces[i] -> centroid, point), i};

    distance_index_pair closest{INF, -1};
    for (int i = 0; i < numSample; i++)
	closest = seq_pair_min(closest, pairs[i]);
    return samplefaces[closest.second];
}

void sequential_locate_recursive(face* curr_face, pt &point, int &result)
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
    {
	result = curr_face -> index;
    }
    else if (next_face -> index < 0)
    {
	result = -1;
    }
    else
    {
	sequential_locate_recursive(next_face, point, result);
    }
}

void DCEL::sequential_locate(pt &point, int &result)
{
    face* start_face = sequential_get_closest_face(point);
    sequential_locate_recursive(start_face, point, result);
}

void DCEL::sequential_locations(std::vector <pt> &points, std::vector <int> &out)
{
    int n = points.size();
    for (int i = 0; i < n; i++)
	sequential_locate(points[i], out[i]);
}
