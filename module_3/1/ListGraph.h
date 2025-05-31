#ifndef LISTGRAPH_H
#define LISTGRAPH_H
#include "IGraph.h"
#include <stdexcept>

struct ListGraph : public IGraph
{
    ListGraph(int verticesCount);
    ListGraph(const IGraph &graph);

    void AddEdge(int from, int to) override;

    int VerticesCount() const override;

    std::vector<int> GetNextVertices(int vertex) const override;
    std::vector<int> GetPrevVertices(int vertex) const override;

private:
    std::vector<std::vector<int>> adjacencyList;
};

ListGraph::ListGraph(int verticesCount)
    : adjacencyList(verticesCount) {}

ListGraph::ListGraph(const IGraph &graph)
    : adjacencyList(graph.VerticesCount())
{
    for (int i = 0; i < adjacencyList.size(); ++i)
    {
        adjacencyList[i] = graph.GetNextVertices(i);
    }
}

void ListGraph::AddEdge(int from, int to)
{
    if (from < 0 || from >= adjacencyList.size() || to < 0 || to >= adjacencyList.size())
    {
        throw std::out_of_range("Vertex index out of range");
    }
    adjacencyList[from].push_back(to);
}

int ListGraph::VerticesCount() const
{
    return adjacencyList.size();
}

std::vector<int> ListGraph::GetNextVertices(int vertex) const
{
    if (vertex < 0 || vertex >= adjacencyList.size())
    {
        throw std::out_of_range("Vertex index out of range");
    }
    return adjacencyList[vertex];
}

std::vector<int> ListGraph::GetPrevVertices(int vertex) const
{
    if (vertex < 0 || vertex >= adjacencyList.size())
    {
        throw std::out_of_range("Vertex index out of range");
    }
    std::vector<int> prevVertices;
    for (int i = 0; i < adjacencyList.size(); ++i)
    {
        for (int next : adjacencyList[i])
        {
            if (next == vertex)
            {
                prevVertices.push_back(i);
            }
        }
    }
    return prevVertices;
}

#endif
