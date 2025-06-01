// Федуков Александр WEB-13.
// orlando20056@yandex.ru
//
// Problem:
/*
Дан невзвешенный неориентированный граф. В графе может быть несколько кратчайших путей между какими-то вершинами. Найдите количество различных кратчайших путей между заданными вершинами. Требуемая сложность O(V+E).
*/

#include <iostream>
#include <vector>
#include <stdexcept>
#include <queue>

////  Graph  ////
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

////  End of Graph  ////

// Find the number of shortest paths using BFS
void BFS(const IGraph &graph, int start, int end, std::vector<int> &distances, std::vector<int> &ways)
{
    std::queue<int> queue;
    queue.push(start);
    distances[start] = 0;
    ways[start] = 1;

    while (!queue.empty())
    {
        int vertex = queue.front();
        queue.pop();

        for (int neighbor : graph.GetNextVertices(vertex))
        {
            if (distances[neighbor] == -1) // Not visited
            {
                distances[neighbor] = distances[vertex] + 1;
                ways[neighbor] = ways[vertex];
                queue.push(neighbor);
            }
            else if (distances[neighbor] == distances[vertex] + 1) // Found another shortest path
            {
                ways[neighbor] += ways[vertex];
            }
        }
    }
}

int CountShortestPaths(const IGraph &graph, int start, int end)
{
    int verticesCount = graph.VerticesCount();
    std::vector<int> distances(verticesCount, -1); // distance from start to vertex
    std::vector<int> ways(verticesCount, 0);       // number of ways to vertex

    BFS(graph, start, end, distances, ways);

    return ways[end];
}

int main(int argc, char const *argv[])
{
    // Input graph
    int v, n; // v - vertices count, n - edges count
    std::cin >> v >> n;
    ListGraph graph(v);
    for (int i = 0; i < n; ++i)
    {
        int from, to;
        std::cin >> from >> to;
        graph.AddEdge(from, to);
        graph.AddEdge(to, from); // Undirected graph
    }

    // Find the number of shortest paths between two vertices
    int start, end;
    std::cin >> start >> end;
    try
    {
        int result = CountShortestPaths(graph, start, end);
        std::cout << result << std::endl;
    }
    catch (const std::out_of_range &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
