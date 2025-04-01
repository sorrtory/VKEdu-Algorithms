// Федуков Александр WEB-13.
// orlando20056@yandex.ru
//
// Problem:
/*
Дано множество целых чисел из [0..109] размера n.

Используя алгоритм поиска k-ой порядковой статистики, требуется найти следующие параметры множества:

10% перцентиль
медиана
90% перцентиль
Требования:

К дополнительной памяти: O(n).
Среднее время работы: O(n)
Должна быть отдельно выделенная функция partition.
Рекурсия запрещена.
Решение должно поддерживать передачу функции сравнения снаружи.
Формат ввода
Сначала вводится кол-во элементов массива n. После сам массив.

6_1. Реализуйте стратегию выбора опорного элемента “медиана трёх”. Функцию Partition реализуйте методом прохода двумя итераторами от начала массива к концу.

Формат вывода
Параметры множества в порядке:

10% перцентиль
медиана
90% перцентиль

*/

#include <iostream>
#include <functional>

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

///////////////////////////////////////////////////////

template <template <typename> class Container, typename T, typename Comparator = std::less<T>>
size_t partition(Container<T> &arr, size_t L, size_t R, size_t pivotIndex, Comparator cmp = Comparator())
{
    std::swap(arr[pivotIndex], arr[R]); // Move pivot to the end
    size_t i = L;

    for (size_t j = L; j < R; ++j)
    {
        if (!cmp(arr[R], arr[j]))
        {
            std::swap(arr[i], arr[j]);
            i++;
        }
    }
    std::swap(arr[i], arr[R]);
    return i;
}

template <template <typename> class Container, typename T, typename Comparator = std::less<T>>
T select(Container<T> &arr, size_t L, size_t R, size_t k, Comparator cmp = Comparator())
{
    while (true)
    {
        if (L == R)
            return arr[L];

        size_t pivotIndex = pickPivot(arr, L, R);
        pivotIndex = partition(arr, L, R, pivotIndex, cmp);

        if (k == pivotIndex)
            return arr[k];
        else if (k < pivotIndex)
            R = pivotIndex - 1;
        else
            L = pivotIndex + 1;
    }
}

template <template <typename> class Container, typename T, typename Comparator = std::less<T>>
size_t pickPivot(Container<T> &arr, size_t L, size_t R, Comparator cmp = Comparator())
{
    // Median of three strategy for pivot selection
    size_t mid = L + (R - L) / 2;
    if (cmp(arr[mid], arr[L]))
        std::swap(arr[L], arr[mid]);
    if (cmp(arr[R], arr[L]))
        std::swap(arr[L], arr[R]);
    if (cmp(arr[R], arr[mid]))
        std::swap(arr[mid], arr[R]);

    return mid;
}

int main(int argc, char const *argv[])
{
    int n;
    std::cin >> n;

    Vector<int> arr;
    for (int i = 0; i < n; ++i)
    {
        int value;
        std::cin >> value;
        arr.push_back(value);
    }

    size_t p10 = select(arr, 0, arr.size() - 1, n / 10);
    size_t median = select(arr, 0, arr.size() - 1, n / 2);
    size_t p90 = select(arr, 0, arr.size() - 1, 9 * n / 10);

    std::cout << p10 << std::endl
              << median << std::endl
              << p90 << std::endl;

    return 0;
}
