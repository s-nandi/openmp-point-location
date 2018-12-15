#ifndef DCEL_H_DEFINED
#define DCEL_H_DEFINED

#include <vector>
#include <algorithm>
#include "pt.h"

struct face;
struct vertex;
struct halfedge;

typedef std::pair<ptT, int> distance_index_pair;
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

    static const int MAX_SAMPLE_SIZE = 100000;
    
    std::vector <face*> faces;
    std::vector <vertex*> vertices;
    std::vector <halfedge*> edges;
    face* exterior;

    int numSample;
    face* samplefaces[MAX_SAMPLE_SIZE];

    vertex* getNewVertex(pt &point);
    halfedge* getNewEdge();
    face* getNewFace(int id = 0);

    void createPointSet(std::vector <pt> &points);
    void buildFaces(std::vector <std::vector<int>> &triangles, std::vector <endpoint_indices> &created_edges);
    void matchTwinEdges(std::vector <endpoint_indices> &created_edges, std::vector <endpoint_indices> &twinless_edges);
    void buildExterior(std::vector <endpoint_indices> &twinless_edges);

    face* sequential_get_closest_face(pt &point);
    void sequential_locate(pt &point, int &result);
    face* parallel_get_closest_face(pt &point);
    void parallel_locate(pt &point, int &result);
    void hybrid_locate(pt &point, int &result);
public:
    DCEL(){}
    DCEL(std::vector <pt> &points, std::vector <std::vector<int>> &triangles);
    void make_DCEL(std::vector <pt> &points, std::vector <std::vector<int>> &triangles);
    ~DCEL();

    void sequential_locations(std::vector <pt> &points, std::vector <int> &out);
    void parallel_locations(std::vector <pt> &points, std::vector <int> &out);
    void hybrid_locations(std::vector <pt> &points, std::vector <int> &out);
};

#endif // DCEL_H_DEFINED
