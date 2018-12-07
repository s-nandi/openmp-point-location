#include <iostream>
#include "dcel.h"

using namespace std;

int main()
{
    std::vector <pt> points = {{0, 0}, {0, 5}, {-5, 5}, {-5, 0}};
    std::vector <vector<int>> faces = {{0, 1, 2}, {2, 3, 0}};
    DCEL dcel;
    dcel.buildFromTriangles(points, faces);

    for (auto face: dcel.faces)
    {
	std::cout << "Face: " << (face -> index) << std::endl;
	for (auto edge: face -> getEdges())
	{
	    std::cout << "Origin: " << (edge -> origin -> point) << " Endpoint: " << (edge -> endpoint() -> point) << std::endl;
	}
    }
}
