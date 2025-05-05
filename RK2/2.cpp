// Федуков Александр WEB-13.
// orlando20056@yandex.ru
//
// Problem: 
/*
Напишите функцию, вычисляющую количество вершин на кратчайшем пути от корня дерева до листа.
Значения - int.

Шаблоны не обязательны.
Рекурсия разрешена.

Формат ввода
По очереди вводятся узлы бинарного дерева в порядке вставки.

Формат вывода
Cамое короткое расстояние от корня до листа.
*/

#include <iostream>
using namespace std;

struct Node {
    int value;
    Node* left;
    Node* right;
    Node(int val) : value(val), left(nullptr), right(nullptr) {}
};

Node* insert(Node* root, int val) {
    if (!root)
        return new Node(val);
    if (val < root->value)
        root->left = insert(root->left, val);
    else
        root->right = insert(root->right, val);
    return root;
}

int minDepth(Node* root) {
    if (!root)
        return 0;
    if (!root->left && !root->right)
        return 1;
    if (!root->left)
        return 1 + minDepth(root->right);
    if (!root->right)
        return 1 + minDepth(root->left);
    return 1 + min(minDepth(root->left), minDepth(root->right));
}

int main() {
    Node* root = nullptr;
    int x;
    while (cin >> x) {
        root = insert(root, x);
    }

    cout << minDepth(root) << endl;

    return 0;
}
