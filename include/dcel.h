
#ifndef DCEL_H_DEFINED
#define DCEL_H_DEFINED

#include <vector>
#include <algorithm>
#include "pt.h"

struct face;
struct vertex;
struct halfedge;

class DCEL
{
private:  
    struct endpoint_indices
    {
        std::pair <int, int> indices;
        halfedge *edge;

        endpoint_indices(int i, int j, halfedge* &e) : edge(e)
        {
            indices = std::minmax(i, j);
        }
    };
    
    std::vector <face*> faces;
    std::vector <vertex*> vertices;
    std::vector <halfedge*> edges;
    face* exterior;

    vertex* getNewVertex(pt &point);
    halfedge* getNewEdge();
    face* getNewFace(int id = 0);

    void createPointSet(std::vector <pt> &points);
    void buildFaces(std::vector <std::vector<int>> &triangles, std::vector <endpoint_indices> &created_edges);
    void matchTwinEdges(std::vector <endpoint_indices> &created_edges, std::vector <endpoint_indices> &twinless_edges);
    void buildExterior(std::vector <endpoint_indices> &twinless_edges);
public:
    DCEL(){}
    DCEL(std::vector <pt> &points, std::vector <std::vector<int>> &triangles);
    void make_DCEL(std::vector <pt> &points, std::vector <std::vector<int>> &triangles);
    ~DCEL();
    int sequential_locate(const pt &point);
    int parallel_locate(const pt &point);
};

#endif // DCEL_H_DEFINED
