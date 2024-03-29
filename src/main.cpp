#include <iostream>
#include <tuple>
#include <assert.h>
#include "dcel.h"
#include "timer.h"
#include "rng.h"

using namespace std;

typedef std::vector <pt> point_set;
typedef std::vector <std::vector<int>> indexed_face_set;

// Checks if p is inside faces[face_index], the actual coordinates of the face are given in points
bool check_point_in_face(const point_set &points, const indexed_face_set &faces, const pt &p, int face_index)
{
    if (face_index < 0)
    {
	for (int i = 0; i < faces.size(); i++)
	{
	    if (check_point_in_face(points, faces, p, i))
		return false;
	}
	return true;
    }
    else
    {
	std::vector <pt> face;
	for (int point_index: faces[face_index])
	    face.push_back(points[point_index]);
	return pt::contains(face, p);
    }
}

// returns triangulation of N x N square grid where each pair of triangles makes up a unit square
// the lower left corner is at (0, 0) and the upper right corner is at (N, N)
std::pair <point_set, indexed_face_set> make_triangular_mesh(int N)
{
    point_set points((N + 1) * (N + 1)); // (x, y) is stored at index x * (N + 1) + y of points
    auto getIndex = [N](int x, int y)
		    {
			return x * (N + 1) + y;
		    };
    
    int current_index = 0;
    for (int x = 0; x <= N; x++)
    {
	for (int y = 0; y <= N; y++)
	{
	    assert((x * (N + 1) + y) == current_index);
	    points[current_index] = {ptT(x), ptT(y)};
	    current_index++;
	}
    }

    indexed_face_set faces;
    for (int x = 0; x < N; x++)
    {
	for (int y = 0; y < N; y++)
	{
	    // Create the triangles in the unit square with a lower left corner of (x, y)
	    int square_indices[4] = {getIndex(x, y),
				     getIndex(x + 1, y),
				     getIndex(x + 1, y + 1),
				     getIndex(x, y + 1)};
	    for (int i = 0; i < 4; i++)
	    {
		if (square_indices[i] >= points.size())
		{
		    cout << "At (x, y): " << x << " " << y << " i: " << i << endl;
		    cout << "Index: " << square_indices[i] << endl;
		}
		assert(square_indices[i] < points.size());
	    }
	    faces.push_back({square_indices[0], square_indices[1], square_indices[2]});
	    faces.push_back({square_indices[2], square_indices[3], square_indices[0]});
	}
    }
    return {points, faces};
}

// returns Q query points for a mesh of a N x N grid
std::vector <pt> make_query_points(int N, int Q)
{
    std::vector <pt> queries(Q);
    random_point_generator rng(0, N);
    for (int i = 0; i < Q; i++)
	queries[i] = rng.getPoint();
    return queries;
}

/*
 * Randomly generates Q query points for a N x N mesh grid and times the following:
 * 1) Building the point set and face indices
 * 2) Making the DCEL
 * 3) Locating the Q points
 * Optionally (if checkCorrectness is true)checks that the locator is correct by using a point in polygon checker
 * Debug flag toggles whether the query points are printed, and whether the located face indices  are printed
 */
void test(int N, int Q, bool checkCorrectness = false, bool debug = false)
{
    point_set points;
    indexed_face_set faces;
    tie(points, faces) = make_triangular_mesh(N);
    DCEL dcel, dcel2;
    // Sequential Building Benchmarking
    {
	timer stopwatch("Sequential DCEL Building " + to_string(Q));
	dcel.make_DCEL(points, faces);
    }
    // Parallel Building Benchmarking
    {
	timer stopwatch("Parallel DCEL Building " + to_string(Q));
	dcel2.parallel_make_DCEL(points, faces);
    }
    
    auto queries = make_query_points(N, Q);
    std::vector <int> sequential_located(Q), parallel_located(Q), hybrid_located(Q);
    if (debug)
    {
	std::cout << "Query points: " << std::endl;
	for (auto &q: queries)
	    std::cout << q << std::endl;
    }
    
    // Sequental Location Benchmarking
    {
	timer stopwatch("Sequential Point Location " + to_string(Q));
	dcel.sequential_locations(queries, sequential_located);
    }    
    // Hybrid Location Benchmarking
    {
	timer stopwatch("Hybrid Point Location " + to_string(Q));
	dcel.hybrid_locations(queries, hybrid_located);
    }
    // Parallel Location Benchmarking
    {
	timer stopwatch("Parallel Point Location " + to_string(Q));
	dcel.parallel_locations(queries, parallel_located);
    }

    if (debug)
    {
	if (sequential_located != parallel_located)
	{
	    for (int i = 0; i < Q; i++)
	    {
		std::cout << "Sequentially located" << queries[i] << " in face " << sequential_located[i] << std::endl;
		std::cout << "Parallely located" << queries[i] << " in face " << parallel_located[i] << std::endl;
	    }
	}
    }
    assert(sequential_located == parallel_located and sequential_located == hybrid_located);
    std::cout << std::endl;

    if (!checkCorrectness)
	return;

    std::vector <int> dcel2_located(Q);
    dcel2.hybrid_locations(queries, dcel2_located);
    assert(sequential_located == dcel2_located);

    int numOutside = 0, numInside = 0;
    for (int i = 0; i < Q; i++)
    {
	if (sequential_located[i] < 0)
	    numOutside++;
	else
	    numInside++;
	if (debug)
	    std::cout << "Located " << queries[i] << " in face " << sequential_located[i] << std::endl;
	assert(check_point_in_face(points, faces, queries[i], sequential_located[i]));
    }
    std::cout << "Inside: " << numInside << " Outside: " << numOutside << std::endl;
    std::cout << "All tests passed!" << std::endl << std::endl;
}

int main()
{
    // Check correctness once
    test(10, 100, true);
    
    // Then perform benchmarks
    for (int N: {100, 200, 300, 400, 500, 600})
	test(N, N * N);
}
