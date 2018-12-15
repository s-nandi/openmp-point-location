#include "dcel.h"
#include "vertex.h"
#include "halfedge.h"
#include "face.h"
#include <limits>

face* DCEL::parallel_get_closest_face(const pt &point)
{
    int n = faces.size();
    int step = pow(n, 1.0 / 4.0);

    int closest = 0;
    ptT min_distance = std::numeric_limits<ptT>::max();
    #pragma omp parallel shared(min_distance) shared(closest)
    {
        #pragma omp for reduction(min:min_distance)
	for (int i = 0; i < numSample; i++)
	{
	    ptT possible_distance = pt::distance2(samplefaces[i] -> centroid, point);
	    min_distance = (possible_distance < min_distance) ? possible_distance : min_distance;
	}
	#pragma omp for
	for (int i = 0; i < numSample; i++)
	{
	    ptT possible_distance = pt::distance2(samplefaces[i] -> centroid, point);
	    if (possible_distance - 1e-6 <= min_distance)
		closest = i;
	}
    }
    return samplefaces[closest];
}

int DCEL::parallel_locate(const pt &point)
{
    face* curr_face = parallel_get_closest_face(point);
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

void DCEL::parallel_locations(const std::vector <pt> &points, std::vector <int> &out)
{
    int n = points.size();
    #pragma omp parallel
    #pragma omp single
    for (int i = 0; i < n; i++)
    {
	#pragma omp task
	out[i] = parallel_locate(points[i]);
    }
}
