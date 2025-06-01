// Федуков Александр WEB-13.
// orlando20056@yandex.ru
//
// Problem: 
/*
Требуется отыскать самый короткий маршрут между городами. Из города может выходить дорога, которая возвращается в этот же город.

Требуемое время работы O((N + M)log N), где N – количество городов, M – известных дорог между ними.
*/

#include <iostream>
#include <vector>
#include <stdexcept>
#include <set>
#include <climits>

////  Graph  ////
struct IGraph
{
    virtual ~IGraph() {}

    virtual void AddEdge(int from, int to, int weight) = 0;

    virtual int VerticesCount() const = 0;

    virtual std::vector<std::pair<int, int>> GetNextVertices(int vertex) const = 0;
    virtual std::vector<std::pair<int, int>> GetPrevVertices(int vertex) const = 0;
};

struct ListGraph : public IGraph
{
    ListGraph(int verticesCount);
    ListGraph(const IGraph &graph);

    void AddEdge(int from, int to, int weight) override;

    int VerticesCount() const override;

    std::vector<std::pair<int, int>> GetNextVertices(int vertex) const override;
    std::vector<std::pair<int, int>> GetPrevVertices(int vertex) const override;

private:
    // Vector(graph) of vectors(vertex) of pairs(to, weight)
    std::vector<std::vector<std::pair<int, int>>> adjacencyList;
};

ListGraph::ListGraph(int verticesCount)
{
    if (verticesCount <= 0)
    {
        throw std::invalid_argument("Number of vertices must be positive");
    }
    adjacencyList.resize(verticesCount);
}

ListGraph::ListGraph(const IGraph &graph)
{
    int count = graph.VerticesCount();
    adjacencyList.resize(count);

    for (int i = 0; i < count; ++i)
    {
        for (const auto& neighbor : graph.GetNextVertices(i))
        {
            adjacencyList[i].push_back(neighbor);
        }
    }
}

int ListGraph::VerticesCount() const
{
    return adjacencyList.size();
}

void ListGraph::AddEdge(int from, int to, int weight)
{
    if (from < 0 || from >= adjacencyList.size() || to < 0 || to >= adjacencyList.size())
    {
        throw std::out_of_range("Vertex index out of range");
    }
    adjacencyList[from].emplace_back(to, weight);
}

std::vector<std::pair<int, int>> ListGraph::GetNextVertices(int vertex) const
{
    if (vertex < 0 || vertex >= adjacencyList.size())
    {
        throw std::out_of_range("Vertex index out of range");
    }
    return adjacencyList[vertex];
}

std::vector<std::pair<int, int>> ListGraph::GetPrevVertices(int vertex) const
{
    if (vertex < 0 || vertex >= adjacencyList.size())
    {
        throw std::out_of_range("Vertex index out of range");
    }

    std::vector<std::pair<int, int>> prevVertices;
    for (int i = 0; i < adjacencyList.size(); ++i)
    {
        for (const auto& edge : adjacencyList[i])
        {
            if (edge.first == vertex)
            {
                prevVertices.emplace_back(i, edge.second);
            }
        }
    }
    return prevVertices;
}

////  End of Graph  ////


void Dijkstra(const IGraph &graph, int start, std::vector<int> &distances, std::vector<int> &previous)
{
    int verticesCount = graph.VerticesCount();
    distances.assign(verticesCount, INT_MAX);
    previous.assign(verticesCount, -1);
    distances[start] = 0;

    std::set<std::pair<int, int>> queue; // (distance, vertex)
    queue.insert({0, start});

    while (!queue.empty())
    {
        auto [currentDistance, currentVertex] = *queue.begin();
        queue.erase(queue.begin());

        // Don't process if we already found a better path to this vertex
        if (currentDistance > distances[currentVertex])
            continue;

        // Process all neighbors and relax edges
        for (const auto &[neighbor, weight] : graph.GetNextVertices(currentVertex))
        {
            int newDistance = currentDistance + weight;
            if (newDistance < distances[neighbor]) // New vertices (have INT_MAX)
            {                                      // or shorter path found
                // Relaxation
                queue.erase({distances[neighbor], neighbor}); // No existance, no behaviour
                distances[neighbor] = newDistance;
                previous[neighbor] = currentVertex;
                queue.insert({newDistance, neighbor});
            }
        }
    }
}

int GetShortestPath(const IGraph &graph, int start, int end)
{
    std::vector<int> distances;
    std::vector<int> previous;
    Dijkstra(graph, start, distances, previous);

    if (distances[end] == INT_MAX)
    {
        return -1; // No path found
    }
    
    return distances[end];
}

int main(int argc, char const *argv[])
{
    int n, m;
    std::cin >> n >> m;

    ListGraph graph(n);
    for (int i = 0; i < m; ++i)
    {
        int from, to, weight;
        std::cin >> from >> to >> weight;
        graph.AddEdge(from, to, weight);
        graph.AddEdge(to, from, weight);
    }

    int start, end;
    std::cin >> start >> end;
    std::cout << GetShortestPath(graph, start, end) << std::endl;
    return 0;
}
