#include "dcel.h"
#include "vertex.h"
#include "halfedge.h"
#include "face.h"
#include "sort.h"
#include <assert.h>

DCEL::DCEL(std::vector <pt> &points, std::vector <std::vector<int>> &triangles)
{
    make_DCEL(points, triangles);
}

void DCEL::make_DCEL(std::vector <pt> &points, std::vector <std::vector<int>> &triangles)
{
    assert(vertices.empty() and faces.empty() and edges.empty());
    
    createPointSet(points);
    std::vector <endpoint_indices> created_edges; // Used to detect twin half-edges after sorting
    buildFaces(triangles, created_edges);
    std::vector <endpoint_indices> twinless_edges; // Boundary edges are half-edges without twins
    matchTwinEdges(created_edges, twinless_edges);
    buildExterior(twinless_edges);

    assert(vertices.size() == points.size());
}

DCEL::~DCEL()
{
    for (auto &f: faces)
	delete f;
    for (auto &v: vertices)
	delete v;
    for (auto &e: edges)
	delete e;
}

vertex* DCEL::getNewVertex(pt &point)
{
    vertex* new_vertex = new vertex(point, vertices.size());
    vertices.push_back(new_vertex);
    return new_vertex;
}

halfedge* DCEL::getNewEdge()
{
    halfedge* new_edge = new halfedge;
    edges.push_back(new_edge);
    return new_edge;
}

face* DCEL::getNewFace(int id)
{
    face* new_face = new face(id ? id : faces.size());
    faces.push_back(new_face);
    return new_face;
}

void DCEL::createPointSet(std::vector <pt> &points)
{
    for (int i = 0; i < points.size(); i++)
    {
	getNewVertex(points[i]);
    }
}

void DCEL::buildFaces(std::vector <std::vector<int>> &triangles, std::vector <endpoint_indices> &created_edges)
{
    for (int i = 0; i < triangles.size(); i++)
    {
	int indices[3] = {triangles[i][0], triangles[i][1], triangles[i][2]};

	auto orient = pt::orientation(vertices[indices[0]] -> point, vertices[indices[1]] -> point, vertices[indices[2]] -> point);
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

void DCEL::matchTwinEdges(std::vector <endpoint_indices> &created_edges, std::vector <endpoint_indices> &twinless_edges)
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

void DCEL::buildExterior(std::vector <endpoint_indices> &twinless_edges)
{
    exterior = getNewFace(-1);

    std::vector <halfedge*> exterior_edges(vertices.size()); // Used to connect exterior edges together

    for (int i = 0; i < twinless_edges.size(); i++)
    {
	halfedge* reverse_edge = getNewEdge(); // Since twin does not exist in interior, new halfedge must be created on the exterior

	vertex* origin = twinless_edges[i].edge -> destination();
	reverse_edge -> setOrigin(origin); // Set origin of exterior edge
	reverse_edge -> setIncidentFace(exterior);
	reverse_edge -> setTwin(twinless_edges[i].edge);

	exterior_edges[origin -> index] = reverse_edge;
    }

    halfedge* firstEdge = exterior -> startEdge;
    halfedge* currEdge = firstEdge;
    do
    {
	int endIndex = currEdge -> destination() -> index;
	currEdge -> setNext(exterior_edges[endIndex]);
	currEdge = exterior_edges[endIndex];
    }
    while (currEdge != firstEdge);

    for (int i = 0; i < faces.size() - 1; i++)
    {
	faces[i] -> initCentroid();
    }
    numSample = 0;
    int step = pow(faces.size(), 4.0 / 5.0);
    for (int i = 0; i < faces.size(); i += step)
    {
	assert(numSample < MAX_SAMPLE_SIZE);
	samplefaces[numSample] = faces[i];
	numSample++;
    }
}
