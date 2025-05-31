#ifndef ArcGraph_h
#define ArcGraph_h
#include "IGraph.h"
#include <stdexcept>
#include <algorithm>

struct ArcGraph : public IGraph
{
    ArcGraph(int verticesCount);
    ArcGraph(const IGraph &graph);

    void AddEdge(int from, int to) override;

    int VerticesCount() const override;

    std::vector<int> GetNextVertices(int vertex) const override;
    std::vector<int> GetPrevVertices(int vertex) const override;
private:
    std::vector<std::pair<int, int>> adjacencyArcs; // (from, to)
};


ArcGraph::ArcGraph(int verticesCount)
{
    if (verticesCount <= 0)
    {
        throw std::invalid_argument("Number of vertices must be positive");
    }
}

ArcGraph::ArcGraph(const IGraph &graph)
{
    int count = graph.VerticesCount();
    for (int i = 0; i < count; ++i)
    {
        for (int neighbor : graph.GetNextVertices(i))
        {
            adjacencyArcs.emplace_back(i, neighbor);
        }
    }
}

void ArcGraph::AddEdge(int from, int to)
{
    if (from < 0 || from >= VerticesCount() || to < 0 || to >= VerticesCount())
    {
        throw std::out_of_range("Vertex index out of range");
    }
    adjacencyArcs.emplace_back(from, to);
}

int ArcGraph::VerticesCount() const
{   
    if (adjacencyArcs.empty())
    {
        return 0;
    } else {
        // Get the biggest vertex index from arcs
        int max_vertex = 0;
        for (const auto& arc : adjacencyArcs) {
            max_vertex = std::max({max_vertex, arc.first, arc.second});
        }
        return max_vertex + 1;
    }
}

std::vector<int> ArcGraph::GetNextVertices(int vertex) const
{
    if (vertex < 0 || vertex >= VerticesCount())
    {
        throw std::out_of_range("Vertex index out of range");
    }
    std::vector<int> nextVertices;

    for (const auto& arc : adjacencyArcs)
    {
        if (arc.first == vertex)
        {
            nextVertices.push_back(arc.second);
        }
    }
    return nextVertices;
}

std::vector<int> ArcGraph::GetPrevVertices(int vertex) const
{
    if (vertex < 0 || vertex >= VerticesCount())
    {
        throw std::out_of_range("Vertex index out of range");
    }
    std::vector<int> prevVertices;
    for (const auto& arc : adjacencyArcs)
    {
        if (arc.second == vertex)
        {
            prevVertices.push_back(arc.first);
        }
    }
    return prevVertices;
}

#endif