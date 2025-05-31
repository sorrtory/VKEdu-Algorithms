#ifndef SetGraph_h
#define SetGraph_h
#include "IGraph.h"
#include "stdexcept"
#include <set>

struct SetGraph : public IGraph
{
    SetGraph(int verticesCount);
    SetGraph(const IGraph &graph);

    void AddEdge(int from, int to) override;

    int VerticesCount() const override;

    std::vector<int> GetNextVertices(int vertex) const override;
    std::vector<int> GetPrevVertices(int vertex) const override;

private:
    std::vector<std::set<int>> adjacencySet;
};

SetGraph::SetGraph(int verticesCount)
{
    if (verticesCount <= 0)
    {
        throw std::invalid_argument("Number of vertices must be positive");
    }
    adjacencySet.resize(verticesCount);
}

SetGraph::SetGraph(const IGraph &graph)
{
    int count = graph.VerticesCount();
    adjacencySet.resize(count);

    for (int i = 0; i < count; ++i)
    {
        for (int neighbor : graph.GetNextVertices(i))
        {
            adjacencySet[i].insert(neighbor);
        }
    }
}

void SetGraph::AddEdge(int from, int to)
{
    if (from < 0 || from >= adjacencySet.size() || to < 0 || to >= adjacencySet.size())
    {
        throw std::out_of_range("Vertex index out of range");
    }
    adjacencySet[from].insert(to);
}

int SetGraph::VerticesCount() const
{
    return adjacencySet.size();
}

std::vector<int> SetGraph::GetNextVertices(int vertex) const
{
    if (vertex < 0 || vertex >= adjacencySet.size())
    {
        throw std::out_of_range("Vertex index out of range");
    }
    return std::vector<int>(adjacencySet[vertex].begin(), adjacencySet[vertex].end());
}

std::vector<int> SetGraph::GetPrevVertices(int vertex) const
{
    if (vertex < 0 || vertex >= adjacencySet.size())
    {
        throw std::out_of_range("Vertex index out of range");
    }

    std::vector<int> prevVertices;
    for (int i = 0; i < adjacencySet.size(); ++i)
    {
        if (adjacencySet[i].find(vertex) != adjacencySet[i].end())
        {
            prevVertices.push_back(i);
        }
    }
    return prevVertices;
}

#endif
