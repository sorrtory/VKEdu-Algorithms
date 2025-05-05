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
#include <cassert>

template <typename T>
class Vector
{
private:
    T *arr;
    size_t len;
    size_t cap;

public:
    Vector();
    Vector(size_t size);
    ~Vector();
    Vector(const Vector &other);
    Vector &operator=(const Vector &other);
    void push_back(T elem);      // Insert with push_back()
    T pop_back();                // Delete with pop_back()
    T &operator[](size_t index); // Edit and get with operator[]
    size_t size();
    void swap(size_t i, size_t j);
};

template <typename T>
Vector<T>::Vector()
{
    cap = 2;
    arr = (T *)malloc(cap * sizeof(T));
    len = 0;
}

template <typename T>
Vector<T>::Vector(size_t size)
{
    arr = (T *)calloc(size, sizeof(T));
    len = size;
    this->cap = size;
}

template <typename T>
Vector<T>::~Vector()
{
    free(arr);
}

template <typename T>
Vector<T>::Vector(const Vector &other)
{
    arr = (T *)malloc(other.cap * sizeof(T));
    len = other.len;
    cap = other.cap;
    for (size_t i = 0; i < len; i++)
    {
        arr[i] = other.arr[i];
    }
}

template <typename T>
Vector<T> &Vector<T>::operator=(const Vector &other)
{
    if (this == &other)
    {
        return *this;
    }
    free(arr);
    arr = (T *)malloc(other.cap * sizeof(T));
    len = other.len;
    cap = other.cap;
    for (size_t i = 0; i < len; i++)
    {
        arr[i] = other.arr[i];
    }
    return *this;
}

template <typename T>
void Vector<T>::push_back(T elem)
{
    if (len == cap)
    {
        cap *= 2;
        arr = (T *)realloc(arr, cap * sizeof(T));
    }
    arr[len++] = elem;
}

template <typename T>
T Vector<T>::pop_back()
{
    return arr[--len];
}

template <typename T>
T &Vector<T>::operator[](size_t index)
{
    if (index < len)
    {
        return arr[index];
    }
    throw; // Out of range
}

template <typename T>
size_t Vector<T>::size()
{
    return len;
}

template <typename T>
void Vector<T>::swap(size_t i, size_t j)
{
    T temp = arr[i];
    arr[i] = arr[j];
    arr[j] = temp;
}

template <typename T, typename Comporator>
class Heap
{
private:
    Vector<T> arr;
    Comporator cmp;

public:
    Heap(Vector<T> vec);
    ~Heap() = default;
    Heap(const Heap &other) = delete;
    Heap &operator=(const Heap &other) = delete;
    void heapify();
    void insert(T elem);
    void sift_up(size_t index);
    void sift_down(size_t index);
    T extract();
    T top();
    size_t size();
};

template <typename T, typename Comporator>
Heap<T, Comporator>::Heap(Vector<T> vec)
{
    arr = vec;
    cmp = Comporator();
    if (vec.size() > 1)
        heapify();
    
}

template <typename T, typename Comporator>
void Heap<T, Comporator>::heapify()
{
    for (size_t i = arr.size() / 2 - 1; i >= 0; i--)
        sift_down(i);
}

template <typename T, typename Comporator>
void Heap<T, Comporator>::insert(T elem)
{
    arr.push_back(elem);
    sift_up(arr.size() - 1);
}

template <typename T, typename Comporator>
void Heap<T, Comporator>::sift_up(size_t index)
{
    // Swap with parent while cmp
    while (index > 0 && cmp(arr[index], arr[(index - 1) / 2]))
    {
        arr.swap(index, (index - 1) / 2);
        index = (index - 1) / 2;
    }
}

template <typename T, typename Comporator>
void Heap<T, Comporator>::sift_down(size_t index)
{
    // Swap with the right child while cmp
    while (2 * index + 1 < arr.size())
    {
        size_t left = 2 * index + 1;
        size_t right = 2 * index + 2;
        size_t wanted = left;
        if (right < arr.size() && cmp(arr[right], arr[left]))
            wanted = right;
        if (cmp(arr[index], arr[wanted]))
            break;
        arr.swap(index, wanted);
        index = wanted;
    }
}

template <typename T, typename Comporator>
T Heap<T, Comporator>::extract()
{
    // Empty heap
    if (!size())
        throw;

    // One element in heap
    if (size() == 1)
        return arr.pop_back();

    T result = arr[0];
    arr[0] = arr.pop_back();
    sift_down(0);
    return result;
}

template <typename T, typename Comporator>
T Heap<T, Comporator>::top()
{
    return arr[0];
}

template <typename T, typename Comporator>
size_t Heap<T, Comporator>::size()
{
    return arr.size();
}

struct User
{
    long id;
    long traffic;

    User(long id, long traffic)
    {
        this->id = id;
        this->traffic = traffic;
    }

    friend std::ostream &operator<<(std::ostream &out, const User &user)
    {
        out << user.id;
        return out;
    }
};

// External comporator
struct UserComporator
{
    // Basically less
    bool operator()(User a, User b)
    {
        return a.traffic < b.traffic;
    }
};

int main(int argc, char const *argv[])
{
    size_t n, k;
    std::cin >> n >> k;
    Vector<User> users;
    Heap<User, UserComporator> heap(users);
    for (size_t i = 0; i < n; i++)
    {   
        
        long id, traffic;
        std::cin >> id >> traffic;
        heap.insert(User(id, traffic));

        // Keep heap size equal to k
        if (heap.size() == k + 1)
            heap.extract();

    }

    // Print heap
    for (size_t i = 0; i < k; i++)
        std::cout << heap.extract() << " ";

    return 0;
}
