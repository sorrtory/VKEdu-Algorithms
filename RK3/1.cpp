// Федуков Александр WEB-13.
// orlando20056@yandex.ru
//
// Problem: 
/*
Посчитать кол-во компонент связности в неориентированном графе. Граф должен быть реализован в виде класса.
*/

#include <iostream>
#include <vector>
#include <stdexcept>

//// Graph  ////

struct IGraph
{
    virtual ~IGraph() {}

    // Добавление ребра от from к to.
    virtual void AddEdge(int from, int to) = 0;

    virtual int VerticesCount() const = 0;

    virtual std::vector<int> GetNextVertices(int vertex) const = 0;
    virtual std::vector<int> GetPrevVertices(int vertex) const = 0;
};

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

//// END OF Graph ////


void dfs(const IGraph &graph, int vertex, std::vector<bool> &visited)
{
    visited[vertex] = true;
    for (int neighbor : graph.GetNextVertices(vertex))
    {
        if (!visited[neighbor])
        {
            dfs(graph, neighbor, visited);
        }
    }
}

int CountConnectedComponents(const IGraph &graph)
{
    int count = 0;
    int verticesCount = graph.VerticesCount();
    std::vector<bool> visited(verticesCount, false);

    for (int i = 0; i < verticesCount; ++i)
    {
        if (!visited[i])
        {
            dfs(graph, i, visited);
            count++;
        }
    }
    return count;
}

int main(int argc, char const *argv[])
{
    int n, m;
    std::cin >> n >> m;
    ListGraph graph(n);
    for (size_t i = 0; i < m; i++)
    {
        int from, to;
        std::cin >> from >> to;
        if (from < 0 || from >= n || to < 0 || to >= n)
        {
            std::cerr << "Vertex index out of range" << std::endl;
            return 1;
        }
        graph.AddEdge(from, to);
        graph.AddEdge(to, from);
    }
        
    std::cout << CountConnectedComponents(graph) << std::endl;
    return 0;
}
