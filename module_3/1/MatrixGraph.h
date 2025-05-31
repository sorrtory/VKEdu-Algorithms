#ifndef MatrixGraph_h
#define MatrixGraph_h
#include "IGraph.h"
#include <stdexcept>

struct MatrixGraph : public IGraph
{
public:
    MatrixGraph(int verticesCount);
    MatrixGraph(const IGraph &graph);

    void AddEdge(int from, int to) override;

    int VerticesCount() const override;

    std::vector<int> GetNextVertices(int vertex) const override;
    std::vector<int> GetPrevVertices(int vertex) const override;

private:
    std::vector<std::vector<bool>> adjacencyMatrix;
};

MatrixGraph::MatrixGraph(int verticesCount)
{
    if (verticesCount <= 0)
    {
        throw std::invalid_argument("Number of vertices must be positive");
    }
    adjacencyMatrix.resize(verticesCount, std::vector<bool>(verticesCount, false));
}

MatrixGraph::MatrixGraph(const IGraph &graph)
{
    int count = graph.VerticesCount();
    adjacencyMatrix.resize(count, std::vector<bool>(count, false));

    for (int i = 0; i < count; ++i)
    {
        for (int neighbor : graph.GetNextVertices(i))
        {
            adjacencyMatrix[i][neighbor] = true;
        }
    }
}

void MatrixGraph::AddEdge(int from, int to)
{
    if (from < 0 || from >= adjacencyMatrix.size() || to < 0 || to >= adjacencyMatrix.size())
    {
        throw std::out_of_range("Vertex index out of range");
    }
    adjacencyMatrix[from][to] = true;
}

int MatrixGraph::VerticesCount() const
{
    return adjacencyMatrix.size();
}

std::vector<int> MatrixGraph::GetNextVertices(int vertex) const
{
    if (vertex < 0 || vertex >= adjacencyMatrix.size())
    {
        throw std::out_of_range("Vertex index out of range");
    }

    std::vector<int> nextVertices;
    for (int i = 0; i < adjacencyMatrix.size(); ++i)
    {
        if (adjacencyMatrix[vertex][i])
        {
            nextVertices.push_back(i);
        }
    }
    return nextVertices;
}

std::vector<int> MatrixGraph::GetPrevVertices(int vertex) const
{
    if (vertex < 0 || vertex >= adjacencyMatrix.size())
    {
        throw std::out_of_range("Vertex index out of range");
    }

    std::vector<int> prevVertices;
    for (int i = 0; i < adjacencyMatrix.size(); ++i)
    {
        if (adjacencyMatrix[i][vertex])
        {
            prevVertices.push_back(i);
        }
    }
    return prevVertices;
}

#endif