// Федуков Александр WEB-13.
// orlando20056@yandex.ru
//
// Problem: 
/*
Рик и Морти снова бороздят просторы вселенных, но решили ограничиться только теми, номера которых меньше M.
Они могут телепортироваться из вселенной с номером z во вселенную (z+1)modM за a бутылок лимонада или во вселенную (z^2+1)modM за b бутылок лимонада.
Рик и Морти хотят добраться из вселенной с номером x во вселенную с номером y.
Сколько бутылок лимонада отдаст Рик за такое путешествие, если он хочет потратить их как можно меньше? Граф должен быть реализован в виде класса.

Формат ввода:
В строке подряд даны количество бутылок a за первый тип телепортации, количество бутылок b за второй тип телепортации, количество вселенных M, номер стартовой вселенной x, номер конечной вселенной y (0≤a,b≤100, 1≤M≤10^6, 0≤x,y<M).

Формат вывода:
Выведите одно число — количество бутылок лимонада, которые отдаст Рик за такое путешествие.
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
    int a, b, m, x, y;
    std::cin >> a >> b >> m >> x >> y;
    
    ListGraph graph(m);
    for (int i = 0; i < m; ++i)
    {
        int to1 = (i + 1) % m;
        int to2 = (static_cast<long long>(i) * i + 1) % m; // 10^12 is bad?
        graph.AddEdge(i, to1, a);
        
        // Avoid duplicates
        if (to2 != to1 || a != b) {
            graph.AddEdge(i, to2, b);
        }
    }
    std::cout << GetShortestPath(graph, x, y) << std::endl;

    return 0;
}
