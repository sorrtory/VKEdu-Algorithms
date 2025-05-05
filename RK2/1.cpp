// Федуков Александр WEB-13.
// orlando20056@yandex.ru
//
// Problem: 
/*
Напишите функцию, проверяющую, что во всех узлах бинарного дерева поиска хранится одинаковое значение.
Значения - int.

Шаблоны не обязательны.
Рекурсия разрешена.

Формат ввода
По очереди вводятся узлы бинарного дерева в порядке вставки.

Формат вывода
0 - если все значения в узлах бинарного дерева разные.
1 - если все значения в узлах бинарного дерева одинаковые.
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

bool checkSame(Node* root, int val) {
    if (!root)
        return true;
    if (root->value != val)
        return false;
    return checkSame(root->left, val) && checkSame(root->right, val);
}

int main() {
    Node* root = nullptr;
    int x;
    while (cin >> x) {
        root = insert(root, x);
    }

    if (!root) {
        cout << 1 << endl;
        return 0;
    }

    if (checkSame(root, root->value))
        cout << 1 << endl;
    else
        cout << 0 << endl;

    return 0;
}
