// Федуков Александр WEB-13.
// orlando20056@yandex.ru
//
// Problem:
/*
Необходимо написать несколько реализаций интерфейса:
ListGraph, хранящий граф в виде массива списков смежности,
MatrixGraph, хранящий граф в виде матрицы смежности,
SetGraph, хранящий граф в виде массива хэш-таблиц/сбалансированных деревьев поиска,
ArcGraph, хранящий граф в виде одного массива пар {from, to}.
Также необходимо реализовать конструктор, принимающий const IGraph&. Такой конструктор должен скопировать переданный граф в создаваемый объект.
Для каждого класса создавайте отдельные h и cpp файлы.
Число вершин графа задается в конструкторе каждой реализации.
*/

#include "ListGraph.h"
#include "MatrixGraph.h"
#include "SetGraph.h"
#include "ArcGraph.h"
#include <iostream>
#include <functional>
#include <queue>

void BFS(const IGraph &graph, int vertex, std::vector<bool> &visited, const std::function<void(int)> &visit)
{
    std::queue<int> queue;
    queue.push(vertex);
    visited[vertex] = true;

    while (!queue.empty())
    {
        int currentVertex = queue.front();
        queue.pop();
        visit(currentVertex);

        for (int neighbor : graph.GetNextVertices(currentVertex))
        {
            if (!visited[neighbor])
            {
                visited[neighbor] = true;
                queue.push(neighbor);
            }
        }
    }
}

void mainBFS(const IGraph &graph, const std::function<void(int)> &visit)
{
    std::vector<bool> visited(graph.VerticesCount(), false);

    for (int startVertex = 0; startVertex < graph.VerticesCount(); ++startVertex)
    {
        if (!visited[startVertex])
        {
            BFS(graph, startVertex, visited, visit);
        }
    }
}

void DFS(const IGraph &graph, int vertex, std::vector<bool> &visited, const std::function<void(int)> &visit)
{
    visited[vertex] = true;
    visit(vertex);

    for (int neighbor : graph.GetNextVertices(vertex))
    {
        if (!visited[neighbor])
        {
            DFS(graph, neighbor, visited, visit);
        }
    }
}

void mainDFS(const IGraph &graph, const std::function<void(int)> &visit)
{
    std::vector<bool> visited(graph.VerticesCount(), false);

    for (int startVertex = 0; startVertex < graph.VerticesCount(); ++startVertex)
    {
        if (!visited[startVertex])
        {
            DFS(graph, startVertex, visited, visit);
        }
    }
}

void topologicalSortInternal(const IGraph &graph, int vertex, std::vector<bool> &visited, std::deque<int> &sorted)
{
    visited[vertex] = true;

    for (int neighbor : graph.GetNextVertices(vertex))
    {
        if (!visited[neighbor])
        {
            topologicalSortInternal(graph, neighbor, visited, sorted);
        }
    }

    sorted.push_front(vertex);
} 

std::deque<int> topologicalSort(const IGraph &graph)
{
    std::deque<int> sorted;
    std::vector<bool> visited(graph.VerticesCount(), false);

    for (int i = 0; i < graph.VerticesCount(); i++)
    {
        if (!visited[i])
        {
            topologicalSortInternal(graph, i, visited, sorted);
        }
        
    }
    return sorted;
}

void testBFS(const IGraph &graph)
{
    std::cout << "BFS Traversal: ";
    mainBFS(graph, [](int vertex) {
        std::cout << vertex << " ";
    });
    std::cout << std::endl;
}

void testDFS(const IGraph &graph)
{
    std::cout << "DFS Traversal: ";
    mainDFS(graph, [](int vertex) {
        std::cout << vertex << " ";
    });
    std::cout << std::endl;
}

void testTopologicalSort(const IGraph &graph)
{
    std::deque<int> sorted = topologicalSort(graph);
    std::cout << "Topological Sort: ";
    for (int vertex : sorted)
    {
        std::cout << vertex << " ";
    }
    std::cout << std::endl;
}


void testGraph(const IGraph &graph)
{
    std::cout << "Vertices Count: " << graph.VerticesCount() << std::endl;
    testBFS(graph);
    testDFS(graph);
    testTopologicalSort(graph);
    std::cout << "=========================" << std::endl;
}

int main(int argc, char const *argv[])
{
    ListGraph listGraph(7);
    listGraph.AddEdge(0, 1);
    listGraph.AddEdge(0, 5);
    listGraph.AddEdge(1, 2);
    listGraph.AddEdge(1, 3); 
    listGraph.AddEdge(1, 5);
    listGraph.AddEdge(1, 6);
    listGraph.AddEdge(3, 2);
    listGraph.AddEdge(3, 4);
    listGraph.AddEdge(3, 6);
    listGraph.AddEdge(5, 4);
    listGraph.AddEdge(5, 6);
    listGraph.AddEdge(6, 4);
    std::cout << "ListGraph:" << std::endl << std::endl;
    testGraph(listGraph);

    MatrixGraph matrixGraph(listGraph);
    std::cout << "MatrixGraph:" << std::endl << std::endl;
    testGraph(matrixGraph);

    SetGraph setGraph(matrixGraph);
    std::cout << "SetGraph:" << std::endl << std::endl;
    testGraph(setGraph);

    ArcGraph arcGraph(setGraph);
    std::cout << "ArcGraph:" << std::endl << std::endl;
    testGraph(arcGraph);

    return 0;
}
