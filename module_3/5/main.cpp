// Федуков Александр WEB-13.
// orlando20056@yandex.ru
//
// Problem: 
/*
Дан неориентированный связный граф. Требуется найти вес минимального остовного дерева в этом графе.
Вариант 1. С помощью алгоритма Прима.

Формат ввода
Первая строка содержит два натуральных числа n и m — количество вершин и ребер графа соответственно (1 ≤ n ≤ 20000, 0 ≤ m ≤ 100000).
Следующие m строк содержат описание ребер по одному на строке.
Ребро номер i описывается тремя натуральными числами bi, ei и wi — номера концов ребра и его вес соответственно (1 ≤ bi, ei ≤ n, 0 ≤ wi ≤ 100000).

Формат вывода
Выведите единственное целое число - вес минимального остовного дерева.
*/

#include <iostream>
#include <vector>
#include <stdexcept>
#include <algorithm>
#include <map>
#include <stack>

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


//// DSU  ////

class DSU
{
public:
    DSU(size_t size)
    : parent(size), rank(size, 1)
    {
        for (int i = 0; i < size; i++)
        {
            parent[i] = i;
        }
    }
    
    size_t find_set(size_t u)
    {
        std::stack<size_t> stack;
        stack.push(u);
        
        while (parent[u] != u)
        {
            stack.push(parent[u]);
            u = parent[u];
        }
        
        size_t &root = u;
        
        while (!stack.empty())
        {
            parent[stack.top()] = root;
            stack.pop();
        }
        
        return root;
    }
    
    void union_set(size_t u, size_t v)
    {
        u = find_set(u);
        v = find_set(v);
        
        if (u != v)
        {
            if (rank[u] < rank[v])
                std::swap(u, v);
            
            parent[v] = u;
            rank[u] += rank[v];
        }
    }
    
private:
    std::vector<size_t> parent;
    std::vector<size_t> rank;
    
    friend std::ostream& operator<<(std::ostream &out, DSU &dsu);
};

std::ostream& operator<<(std::ostream &out, DSU &dsu)
{
    std::map<size_t, std::vector<size_t>> sets;
    
    for (auto i = 0; i < dsu.parent.size(); i++)
    {
        sets[dsu.find_set(i)].push_back(i);
    }
    
    for (auto &kv: sets)
    {
        out << kv.first << " [rank = " << dsu.rank[kv.first] << "]: ";
        for (auto i = 0; i < kv.second.size(); i++)
        {
            out << kv.second[i];
            if (i != kv.second.size() - 1)
                out << ", ";
        }
        out << std::endl;
    }
    
    return out;
}

//// END OF DSU ////


struct Edge
{
    int from;
    int to;
    int weight;

    bool operator<(const Edge &other) const
    {
        return weight < other.weight;
    }
};


int main(int argc, char const *argv[])
{
    int n, m;
    std::cin >> n >> m;
    std::vector<Edge> edges(m);
    for (int i = 0; i < m; ++i) {
        int b, e, w;
        std::cin >> b >> e >> w;
        --b; --e;
        edges[i] = {b, e, w};
    }

    std::sort(edges.begin(), edges.end());

    DSU dsu(n);
    long long mst_weight = 0;
    int edges_used = 0;
    for (const auto& edge : edges) {
        int u = dsu.find_set(edge.from);
        int v = dsu.find_set(edge.to);
        if (u != v) {
            dsu.union_set(u, v);
            mst_weight += edge.weight;
            edges_used++;
            if (edges_used == n - 1) break;
        }
    }
    std::cout << mst_weight << std::endl;
    return 0;
}
