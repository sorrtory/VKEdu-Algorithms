// Федуков Александр WEB-13.
// orlando20056@yandex.ru
//
// Problem: 
/*
Нужно проверить, является ли путь в неориентированном графе гамильтоновым. Граф должен быть реализован в виде класса.

Формат ввода
Первая строка содержит число N – количество вершин.
Вторая строка содержит число M - количество ребер.
Третья строка содержит число K - количество вершин в пути.
Далее идут M строк с ребрами (откуда, куда).
Далее идут K вершин, принадлежащих пути.

Формат вывода
1 - если путь гамильтонов
0 - если путь не гамильтонов
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

bool IsHamiltonianPath(const IGraph &graph, const std::vector<int> &path)
{
    if (path.empty() || path.size() != graph.VerticesCount())
        return false;

    std::vector<bool> visited(graph.VerticesCount(), false);
    
    // Check if path has only unique vertices and vertices are good
    for (int vertex : path)
    {
        if (vertex < 0 || vertex >= graph.VerticesCount() || visited[vertex])
            return false;
        visited[vertex] = true;
    }

    // Check if path is traversable
    for (size_t i = 1; i < path.size(); ++i)
    {
        bool found = false;
        for (int neighbor : graph.GetNextVertices(path[i - 1]))
        {
            if (neighbor == path[i])
            {
                found = true;
                break;
            }
        }
        if (!found)
            return false;
    }

    return true;
}

int main(int argc, char const *argv[])
{
    int n, m, k;
    std::cin >> n >> m >> k;
    ListGraph graph(n);
    for (int i = 0; i < m; ++i)
    {
        int from, to;
        std::cin >> from >> to;
        graph.AddEdge(from, to);
        graph.AddEdge(to, from);
    }

    std::vector<int> path(k);
    for (int i = 0; i < k; ++i)
    {
        std::cin >> path[i];
    }

    std::cout << (IsHamiltonianPath(graph, path) ? 1 : 0) << std::endl;
    
    return 0;
}
