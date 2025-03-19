// Федуков Александр WEB-13.
// orlando20056@yandex.ru
//
// Problem:
/*
Даны два массива неубывающих целых чисел, упорядоченные по возрастанию. A[0..n-1] и B[0..m-1]. n >> m. Найдите их пересечение.
Требования: Время работы: O(m * log k), где k - позиция элемента B[m-1] в массиве A..
В процессе поиска очередного элемента B[i] в массиве A пользуйтесь результатом поиска элемента B[i-1].
Внимание! В этой задаче для каждого B[i] сначала нужно определить диапазон для бинарного поиска размером порядка k с помощью экспоненциального поиска,
а потом уже в нем делать бинарный поиск.
*/

#include <stdio.h>
#include <cstdlib>

int binarySearch(int target, int left, int right, int *arr)
{

    while (left <= right)
    {
        int n = (right - left) / 2 + left;
        if (arr[n] == target)
            return n;
        if (arr[n] > target)
            right = n - 1;
        if (arr[n] < target)
            left = n + 1;
    }

    return -1;
}

int exponentialSearch(int target, int left, int right, int *arr)
{
    if (arr[left] == target)
        return left;

    int i = 1;
    while (left + i <= right && arr[left + i] <= target)
        i *= 2;

    // Don't exceed the right bound
    if (left + i > right)
        return binarySearch(target, left + i / 2, right, arr);
    else
        return binarySearch(target, left + i / 2, left + i, arr);
}

int main(int argc, char const *argv[])
{
    // Input
    int n, m;
    scanf("%d %d", &n, &m);
    int *A = (int *)malloc(n * sizeof(int));
    for (size_t i = 0; i < n; i++)
        scanf("%d", &A[i]);

    int *B = (int *)malloc(m * sizeof(int));
    for (size_t i = 0; i < m; i++)
        scanf("%d", &B[i]);

    // Search range and number in this range
    int left = 0;
    int right = n - 1;
    for (size_t i = 0; i < m; i++)
    {
        int b = exponentialSearch(B[i], left, right, A);

        if (b != -1)
        {
            printf("%d ", B[i]);
            left = b + 1;
        }
    }

    free(A);
    free(B);
    return 0;
}
