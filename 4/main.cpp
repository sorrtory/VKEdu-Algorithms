// Федуков Александр WEB-13.
// orlando20056@yandex.ru
//
// Problem: 
/*
Имеется лог-файл, в котором хранятся пары для N пользователей (Идентификатор пользователя, посещаемость сайта).
Напишите программу, которая выбирает K пользователей, которые чаще других заходили на сайт, и выводит их в порядке возрастания посещаемости.
Количество заходов и идентификаторы пользователей не повторяются.

Требования:

Время работы O(N * logK)
Куча должна быть реализована в виде шаблонного класса.
Решение должно поддерживать передачу функции сравнения снаружи.
Куча должна быть динамической.
Формат ввода
Сначала вводятся N и K, затем пары (Идентификатор пользователя, посещаемость сайта).

Формат вывода
Идентификаторы пользователей в порядке возрастания посещаемости.
*/

#include <iostream>

template<typename T>
class Vector
{
private:
    T *arr;
    size_t len;
    size_t cap;
public:
    Vector(size_t cap);
    ~Vector();
    Queue(const Queue &other);
    Queue &operator=(const Queue &other);
};

template<typename T>
Vector<T>::Vector(size_t cap)
{
    arr = (*T)malloc(cap * sizeof(T));
    len = 0;
    this->cap = cap;
}

template<typename T>
Vector<T>::~Vector()
{
    free(arr);
}

// TODO minHEAP

int main(int argc, char const *argv[])
{
    int n;
    std::cin >> n;
    
    return 0;
}
