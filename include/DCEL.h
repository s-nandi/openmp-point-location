#include <vector>
#include <iostream>
#include <random>
#include <algorithm>

#include "vertex.h"
#include "halfedge.h"
#include "face.h"
#include "sort.h"

struct DCEL
{
    std::vector <face*> faces;
    std::vector <vertex*> vertices;
    std::vector <halfedge*> edges;
    face* exterior;

    vertex* getNewVertex(pt &point)
    {
        vertex* new_vertex = new vertex(point, vertices.size());
        vertices.push_back(new_vertex);
        return new_vertex;
    }

    halfedge* getNewEdge()
    {
        halfedge* new_edge = new halfedge;
        edges.push_back(new_edge);
        return new_edge;
    }

    face* getNewFace(int id = 0)
    {
        face* new_face = new face(id ? id : faces.size());
        faces.push_back(new_face);
        return new_face;
    }

    void buildFromTriangles(std::vector <pt> &points, std::vector <std::vector<int>> &triangles)
    {
        createPointSet(points);
        std::vector <endpoint_indices> created_edges; // Used to detect twin half-edges after sorting
	buildFaces(triangles, created_edges);
        std::vector <endpoint_indices> twinless_edges; // Boundary edges are half-edges without twins
        matchTwinEdges(created_edges, twinless_edges);
        buildExterior(twinless_edges);
    }

    void createPointSet(std::vector <pt> &points)
    {
        for (int i = 0; i < points.size(); i++)
        {
            vertices[i] = getNewVertex(points[i]);
        }
    }

    struct endpoint_indices
    {
        std::pair <int, int> indices;
        halfedge *edge;

        endpoint_indices(int i, int j, halfedge* &e) : edge(e)
        {
            indices = std::minmax(i, j);
        }
    };

    void buildFaces(std::vector <std::vector<int>> &triangles, std::vector <endpoint_indices> &created_edges)
    {
        for (int i = 0; i < triangles.size(); i++)
        {
            int indices[3] = {triangles[i][0], triangles[i][1], triangles[i][2]};

            auto orient = orientation(vertices[indices[0]] -> point, vertices[indices[1]] -> point, vertices[indices[2]] -> point);
            if (orient > 0) // Ensure that half edges in a face are ccw oriented
            {
                std::swap(indices[0], indices[2]);
            }

            face* curr_face = getNewFace();
            halfedge* triangle_edges[3]; // j-th half edge originates from indices[j]
            for (int j = 0; j < 3; j++)
            {
                triangle_edges[j] = getNewEdge();
            }

            for (int j = 0; j < 3; j++)
            {
                int jnext = (j + 1) % 3;

                triangle_edges[j] -> setIncidentFace(curr_face); // Every edge belongs to same face
                triangle_edges[j] -> setOrigin(vertices[indices[j]]);// Set origin of halfedge and incidentEdge for origin
                triangle_edges[j] -> setNext(triangle_edges[jnext]); // Set next edge for face traversal

                created_edges.push_back({indices[j], indices[jnext], triangle_edges[j]}); // Use vertex indices to detect twin edges later
            }
        }
    }

    void matchTwinEdges(std::vector <endpoint_indices> &created_edges, std::vector <endpoint_indices> &twinless_edges)
    {
        auto radix_sort = [](std::vector <endpoint_indices> &vp, int numPoints)
        {
            auto getSecond = [](endpoint_indices vp){return vp.indices.second;};
            bucket_sort(vp, numPoints, getSecond);
            auto getFirst = [](endpoint_indices vp){return vp.indices.first;};
            bucket_sort(vp, numPoints, getFirst);
        };
        radix_sort(created_edges, vertices.size());
        for (int i = 0; i < created_edges.size(); )
        {
            if (i + 1 < created_edges.size() and created_edges[i].indices == created_edges[i + 1].indices)
            {
                created_edges[i].edge -> setTwin(created_edges[i + 1].edge);
                i += 2;
            }
            else
            {
                twinless_edges.push_back(created_edges[i]);
                i++;
            }
        }
    }

    void buildExterior(std::vector <endpoint_indices> &twinless_edges)
    {
        exterior = getNewFace(-1);

        std::vector <halfedge*> exterior_edges(vertices.size()); // Used to connect exterior edges together

        for (int i = 0; i < twinless_edges.size(); i++)
        {
            halfedge* reverse_edge = getNewEdge(); // Since twin does not exist in interior, new halfedge must be created on the exterior

            vertex* origin = twinless_edges[i].edge -> endpoint();
            reverse_edge -> setOrigin(origin); // Set origin of exterior edge
            reverse_edge -> setIncidentFace(exterior);
            reverse_edge -> setTwin(twinless_edges[i].edge);

            exterior_edges[origin -> index] = reverse_edge;
        }

        halfedge* firstEdge = exterior -> startEdge;
        halfedge* currEdge = firstEdge;
        do
        {
            int endIndex = currEdge -> endpoint() -> index;
            currEdge -> setNext(exterior_edges[endIndex]);
            currEdge = exterior_edges[endIndex];
        }
        while (currEdge != firstEdge);
    }
};
