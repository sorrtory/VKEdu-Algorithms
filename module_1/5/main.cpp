// Федуков Александр WEB-13.
// orlando20056@yandex.ru
//
// Problem:
/*
На числовой прямой окрасили N отрезков.
Известны координаты левого и правого концов каждого отрезка [Li, Ri]. Найти длину окрашенной части числовой прямой.
N ≤ 10000. Li, Ri — целые числа в диапазоне [0, 10**9].

Формат ввода
В первой строке записано количество отрезков.
В каждой последующей строке через пробел записаны координаты левого и правого концов отрезка.

Формат вывода
Выведите целое число — длину окрашенной части.
*/

#include <iostream>

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
    T &operator[](size_t index); // Edit and get with operator[]
    size_t size();
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
void merge(Vector<T> &vec, size_t left, size_t mid, size_t right, bool (*cmp)(const T &l, const T &r))
{
    size_t n1 = mid - left + 1;
    size_t n2 = right - mid;

    Vector<T> L(n1), R(n2);

    for (size_t i = 0; i < n1; i++)
        L[i] = vec[left + i];
    for (size_t j = 0; j < n2; j++)
        R[j] = vec[mid + 1 + j];

    size_t i = 0, j = 0;
    size_t k = left;

    while (i < n1 && j < n2)
    {
        if (cmp(L[i], R[j]))
        {
            vec[k] = L[i];
            i++;
        }
        else
        {
            vec[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1)
    {
        vec[k] = L[i];
        i++;
        k++;
    }

    while (j < n2)
    {
        vec[k] = R[j];
        j++;
        k++;
    }
}

template <typename T>
void mergeSort(Vector<T> &vec, size_t left, size_t right, bool (*cmp)(const T &l, const T &r))
{
    if (left >= right)
        return;

    size_t mid = left + (right - left) / 2;
    mergeSort(vec, left, mid, cmp);
    mergeSort(vec, mid + 1, right, cmp);
    merge(vec, left, mid, right, cmp);
}

struct Point
{
    size_t x;
    int z;
};


int main(int argc, char const *argv[])
{
    size_t n;
    std::cin >> n;
    Vector<Point> points;
    for (size_t i = 0; i < n; i++)
    {
        size_t x1, x2;
        std::cin >> x1 >> x2;
        points.push_back(Point{x1, 1});
        points.push_back(Point{x2, -1});
    }

    mergeSort<Point>(points, 0, points.size() - 1, [](const Point &l, const Point &r) {
        if (l.x == r.x)
            return l.z < r.z;
        return l.x < r.x;
    });
    
    int z = 1;
    size_t length = 0;
    size_t last_x = points[0].x;
    for (size_t i = 1; i < points.size(); i++)
    {
        if (z > 0)
            length += points[i].x - last_x;
        z += points[i].z;
        last_x = points[i].x;
    }
    
    std::cout << length << std::endl;
    return 0;
}
