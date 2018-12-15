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
    {
	timer stopwatch("Making mesh " + to_string(Q));
	tie(points, faces) = make_triangular_mesh(N);
    }
    
    DCEL dcel;
    {
	timer stopwatch("Building DCEL " + to_string(Q));
	dcel.make_DCEL(points, faces);
    }
    
    auto queries = make_query_points(N, Q);
    vector <int> locations(Q);

    if (debug)
    {
	std::cout << "Query points: " << std::endl;
	for (auto &q: queries)
	    std::cout << q << std::endl;
    }

    // Timing block
    {
	timer stopwatch("Point Location " + to_string(Q));
	for (int i = 0; i < Q; i++)
	{
	    locations[i] = dcel.locate(queries[i]);
	}
    }

    if (!checkCorrectness)
	return;

    int numOutside = 0, numInside = 0;
    for (int i = 0; i < Q; i++)
    {
	int located = locations[i];
	if (located < 0)
	    numOutside++;
	else
	    numInside++;
	if (debug)
	    std::cout << "Located " << queries[i] << " in face " << located << std::endl;
	assert(check_point_in_face(points, faces, queries[i], locations[i]));
    }
    std::cout << "Inside: " << numInside << " Outside: " << numOutside << std::endl;
    std::cout << "All tests passed!" << std::endl;
}

int main()
{
    // Check correctness once
    test(10, 10000, true);
    // Test timing
    for (int N: {5, 10, 20, 50, 100, 200})
    {
	cout << endl;
	timer stopwatch("Testing " + to_string(N * N));
	test(N, N * N);
    }
}
