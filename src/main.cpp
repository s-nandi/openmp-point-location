#include <iostream>
#include "dcel.h"
#include "timer.h"

using namespace std;

int main()
{
    timer stopwatch("Main");
    std::vector <pt> points = {{0, 0}, {0, 5}, {-5, 5}, {-5, 0}};
    std::vector <vector<int>> faces = {{0, 1, 2}, {2, 3, 0}};
    DCEL dcel;
    dcel.buildFromTriangles(points, faces);
}
