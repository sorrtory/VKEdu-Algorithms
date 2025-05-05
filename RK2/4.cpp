// Федуков Александр WEB-13.
// orlando20056@yandex.ru
//
// Problem: 
/*
Реализуйте AVL-дерево. Решения с использованием других структур засчитываться не будут.

Входной файл содержит описание операций с деревом. Операций не больше 10^5.
Шаблоны не обязательны.
Рекурсия разрешена.
*/

#include <iostream>
#include <string>
using namespace std;

struct Node {
    int key;
    int height;
    Node* left;
    Node* right;

    Node(int k) : key(k), height(1), left(nullptr), right(nullptr) {}
};

int getHeight(Node* node) {
    return node ? node->height : 0;
}

int getBalance(Node* node) {
    return node ? getHeight(node->right) - getHeight(node->left) : 0;
}

void updateHeight(Node* node) {
    if (node)
        node->height = max(getHeight(node->left), getHeight(node->right)) + 1;
}

Node* rotateRight(Node* y) {
    Node* x = y->left;
    Node* T2 = x->right;

    x->right = y;
    y->left = T2;

    updateHeight(y);
    updateHeight(x);

    return x;
}

Node* rotateLeft(Node* x) {
    Node* y = x->right;
    Node* T2 = y->left;

    y->left = x;
    x->right = T2;

    updateHeight(x);
    updateHeight(y);

    return y;
}

Node* balance(Node* node) {
    updateHeight(node);
    int balanceFactor = getBalance(node);

    if (balanceFactor == 2) {
        if (getBalance(node->right) < 0)
            node->right = rotateRight(node->right);
        return rotateLeft(node);
    }
    if (balanceFactor == -2) {
        if (getBalance(node->left) > 0)
            node->left = rotateLeft(node->left);
        return rotateRight(node);
    }
    return node;
}

Node* insert(Node* node, int key) {
    if (!node)
        return new Node(key);
    if (key < node->key)
        node->left = insert(node->left, key);
    else if (key > node->key)
        node->right = insert(node->right, key);
    // key == node->key: do nothing because the task...
    return balance(node);
}

Node* findMin(Node* node) {
    return node->left ? findMin(node->left) : node;
}

Node* removeMin(Node* node) {
    if (!node->left)
        return node->right;
    node->left = removeMin(node->left);
    return balance(node);
}

Node* remove(Node* node, int key) {
    if (!node) return nullptr;
    if (key < node->key)
        node->left = remove(node->left, key);
    else if (key > node->key)
        node->right = remove(node->right, key);
    else {
        Node* l = node->left;
        Node* r = node->right;
        delete node;
        if (!r) return l;
        Node* min = findMin(r);
        min->right = removeMin(r);
        min->left = l;
        return balance(min);
    }
    return balance(node);
}

bool exists(Node* node, int key) {
    if (!node)
        return false;
    if (key == node->key)
        return true;
    if (key < node->key)
        return exists(node->left, key);
    else
        return exists(node->right, key);
}

Node* next(Node* node, int key) {
    Node* res = nullptr;
    while (node) {
        if (node->key > key) {
            res = node;
            node = node->left;
        } else {
            node = node->right;
        }
    }
    return res;
}

Node* prev(Node* node, int key) {
    Node* res = nullptr;
    while (node) {
        if (node->key < key) {
            res = node;
            node = node->right;
        } else {
            node = node->left;
        }
    }
    return res;
}

int main() {
    Node* root = nullptr;
    string command;
    int x;

    while (cin >> command >> x) {
        if (command == "insert") {
            root = insert(root, x);
        } else if (command == "delete") {
            root = remove(root, x);
        } else if (command == "exists") {
            cout << (exists(root, x) ? "true" : "false") << endl;
        } else if (command == "next") {
            Node* res = next(root, x);
            if (res)
                cout << res->key << endl;
            else
                cout << "none" << endl;
        } else if (command == "prev") {
            Node* res = prev(root, x);
            if (res)
                cout << res->key << endl;
            else
                cout << "none" << endl;
        }
    }

    return 0;
}
