// Федуков Александр WEB-13.
// orlando20056@yandex.ru
//
// Problem:
/*
Дано число N и N строк. Каждая строка содержит команду добавления или удаления натуральных чисел, а также запрос на получение k-ой порядковой статистики. Команда добавления числа A задается положительным числом A, команда удаления числа A задается отрицательным числом “-A”. Запрос на получение k-ой порядковой статистики задается числом k.
*/

#include <iostream>

template <typename T, typename Compare = std::less<T>>
class AvlTree
{
private:
struct Node
{
    Node(const T &data)
        : data(data), left(nullptr), right(nullptr), height(1), count(1)
    {
    }

    T data;
    Node *left;
    Node *right;
    size_t height;
    size_t count;
};

public:
    AvlTree()
        : root(nullptr)
    {
    }

    ~AvlTree()
    {
        destroyTree(root);
    }

    void Add(const T &data)
    {
        root = addInternal(root, data);
    }

    bool Has(const T &data)
    {
        Node *tmp = root;
        while (tmp)
        {
            if (tmp->data == data)
                return true;
            else if (cmp(tmp->data, data))
                tmp = tmp->right;
            else
                tmp = tmp->left;
        }
        return false;
    }
    void Delete(const T &data)
    {
        root = deleteInternal(root, data);
    }

    template <typename U, typename C>
    friend typename AvlTree<U, C>::Node* GetKth(AvlTree<U, C> &tree, size_t k);
private:
    void destroyTree(Node *node)
    {
        if (node)
        {
            destroyTree(node->left);
            destroyTree(node->right);
            delete node;
        }
    }

    Node *deleteInternal(Node *node, const T &data)
    {
        if (!node)
            return nullptr;

        if (node->data == data)
        {
            Node *left = node->left;
            Node *right = node->right;

            delete node;

            if (!right)
                return left;

            // поддерево, из которого берем элемент взамен удаляемого, выбираем на основе сравнения глубин.
            // (берем из более глубокого)

            Node *swapped = nullptr;
            if (getHeight(left) > getHeight(right))
                // Max from left
                swapped = findAndRemoveMax(left, right);
            else
                // Min from right
                swapped = findAndRemoveMin(left, right);
            return doBalance(swapped);
        }
        else if (cmp(node->data, data))
            node->right = deleteInternal(node->right, data);
        else
            node->left = deleteInternal(node->left, data);

        return doBalance(node);
    }

    Node *findAndRemoveMin(Node *left, Node *right)
    {
        Node *min = findMin(right);    // возвращает минимальный элемент в дереве
        min->right = removeMin(right); // возвращает дерево, из которого удалили минимальный элемент
        min->left = left;
        return min;
    }

    Node *findAndRemoveMax(Node *left, Node *right)
    {
        Node *max = findMax(left);   // возвращает максимальный элемент в дереве
        max->left = removeMax(left); // возвращает дерево, из которого удалили максимальный элемент
        max->right = right;
        return max;
    }

    Node *findMin(Node *node)
    {
        while (node->left)
            node = node->left;
        return node;
    }

    Node *removeMin(Node *node)
    {
        if (!node->left)
            return node->right;
        node->left = removeMin(node->left);
        return doBalance(node);
    }

    Node *findMax(Node *node)
    {
        while (node->right)
            node = node->right;
        return node;
    }

    Node *removeMax(Node *node)
    {
        if (!node->right)
            return node->left;
        node->right = removeMax(node->right);
        return doBalance(node);
    }

    Node *addInternal(Node *node, const T &data)
    {
        if (!node)
            return new Node(data);
        if (node->data == data || cmp(node->data, data))
            node->right = addInternal(node->right, data);
        else
            node->left = addInternal(node->left, data);

        return doBalance(node);
    }

    size_t getHeight(Node *node)
    {
        return node ? node->height : 0;
    }

    void fixHeight(Node *node)
    {
        node->height = std::max(getHeight(node->left), getHeight(node->right)) + 1;
    }

    void fixCount(Node *node)
    {
        node->count = getCount(node->left) + getCount(node->right) + 1;
    }

    size_t getCount(Node *node)
    {
        return node ? node->count : 0;
    }

    void fixCountAndHeight(Node *node)
    {
        fixCount(node);
        fixHeight(node);
    }

    int getBalance(Node *node)
    {
        return getHeight(node->right) - getHeight(node->left);
    }

    Node *rotateLeft(Node *node)
    {
        Node *tmp = node->right;
        node->right = tmp->left;
        tmp->left = node;
        fixCountAndHeight(node);
        fixCountAndHeight(tmp);
        return tmp;
    }

    Node *rotateRight(Node *node)
    {
        Node *tmp = node->left;
        node->left = tmp->right;
        tmp->right = node;
        fixCountAndHeight(node);
        fixCountAndHeight(tmp);
        return tmp;
    }

    Node *doBalance(Node *node)
    {
        fixCountAndHeight(node);
        switch (getBalance(node))
        {
        case 2:
        {
            if (getBalance(node->right) < 0)
                node->right = rotateRight(node->right);
            return rotateLeft(node);
        }
        case -2:
        {
            if (getBalance(node->left) > 0)
                node->left = rotateLeft(node->left);
            return rotateRight(node);
        }
        default:
            return node;
        }
    }

    Node *root;
    Compare cmp;
};

template <typename T, typename Compare>
typename AvlTree<T, Compare>::Node *GetKth(AvlTree<T, Compare> &tree, size_t k)
{
    typename AvlTree<T, Compare>::Node *node = tree.root;
    while (node)
    {
        size_t leftCount = tree.getCount(node->left);

        if (k == leftCount) 
            return node;
        else if (tree.cmp(k, leftCount))
            node = node->left;
        else {
            node = node->right;        
            k -= leftCount + 1;
        }
    }
    return nullptr;
}

int main(int argc, const char *argv[])
{
    size_t n;
    std::cin >> n;
    AvlTree<size_t, std::less<size_t>> tree;
    for (size_t i = 0; i < n; ++i)
    {
        long num;
        std::cin >> num;
        if (num > 0)
            tree.Add(num);
        else if (num < 0)
            tree.Delete(-num);

        size_t k;
        std::cin >> k;
        auto node = GetKth(tree, k);
        if (node)
            std::cout << node->data << std::endl;
        else
            std::cout << "NO WAY" << std::endl;
    }
    return 0;
}