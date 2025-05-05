// Федуков Александр WEB-13.
// orlando20056@yandex.ru
//
// Problem:
/*
Дано число N < 10^6 и последовательность целых чисел из [-2^31..2^31] длиной N.
Требуется построить бинарное дерево, заданное наивным порядком вставки.
Т.е., при добавлении очередного числа K в дерево с корнем root, если root→Key ≤ K, то узел K добавляется в правое поддерево root; иначе в левое поддерево root.
Требования: Рекурсия запрещена. Решение должно поддерживать передачу функции сравнения снаружи.

*/

#include <iostream>
#include <stack>
#include <vector>

template <typename T, typename Compare = std::less_equal<T>>
class BinaryTree
{
public:
    struct Node
    {
        T data;
        bool visited;
        Node *left;
        Node *right;

        Node(T v) : data(v), visited(false), left(nullptr), right(nullptr) {}
    };

    BinaryTree() : root(nullptr), cmp(Compare()) {};
    ~BinaryTree();
    std::vector<Node *> post_order(Node *node);
    void Add(T value);
    void Print();

private:
    Compare cmp;
    Node *root = nullptr;
};

template <typename T, typename Compare>
BinaryTree<T, Compare>::~BinaryTree()
{
    for (auto &&node : post_order(root))
        delete node;
    root = nullptr;
}

// Returns a vector of nodes in postorder
// Postorder: left, right, node
template <typename T, typename Compare>
std::vector<typename BinaryTree<T, Compare>::Node *> BinaryTree<T, Compare>::post_order(Node *node)
{
    if (!node)
        node = root;
    if (!node)
        return std::vector<Node *>();
    std::vector<Node *> result;
    std::stack<Node *> stack;

    stack.push(node);
    while (!stack.empty())
    {
        Node *current = stack.top();
        if (current->visited)
        {
            stack.pop();
            result.push_back(current);
        }
        else
        {
            current->visited = true;
            if (current->right)
                stack.push(current->right);
            if (current->left)
                stack.push(current->left);
        }
    }
    return result;
}

template <typename T, typename Compare>
void BinaryTree<T, Compare>::Add(T value)
{
    Node *node = new Node(value);
    if (!root)
        root = node;
    else
    {
        Node *current = root;
        while (true)
        {
            if (cmp(current->data, value))
            {
                if (!current->right)
                {
                    current->right = node;
                    break;
                }
                current = current->right;
            }
            else
            {
                if (!current->left)
                {
                    current->left = node;
                    break;
                }
                current = current->left;
            }
        }
    }
}

template <typename T, typename Compare>
void BinaryTree<T, Compare>::Print()
{
    std::vector<Node *> nodes = post_order(root);
    for (auto node : nodes)
        std::cout << node->data << " ";
}

int main(int argc, char const *argv[])
{
    size_t n;
    std::cin >> n;
    BinaryTree<long, std::less_equal<long>> tree;
    for (size_t i = 0; i < n; ++i)
    {
        long value;
        std::cin >> value;
        tree.Add(value);
    }
    tree.Print();

    return 0;
}
