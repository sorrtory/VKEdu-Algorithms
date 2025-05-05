// Федуков Александр WEB-13.
// orlando20056@yandex.ru
//
// Problem: 
/*
Дан массив неотрицательных целых 64-разрядных чисел. Количество чисел не больше 1000000. Отсортировать массив методом MSD по битам (бинарный QuickSort).
*/

#include <iostream>
#include <cinttypes>


void printBits(uint64_t *arr, size_t n)
{
    for (size_t i = 0; i < n; i++)
    {
        for (int bit = 63; bit >= 0; bit--)
        {
            std::cout << ((arr[i] >> bit) & 1);
        }
        std::cout << std::endl;
    }
}

// Sort by nth bits like 0, 0... 1, 1...
// Return the index where 1s start
size_t partition(uint64_t *arr, size_t L, size_t R, size_t bit)
{
    size_t i = L;
    size_t j = R;

    while (i < j)
    {
        while (i < j && ((arr[i] >> bit) & 1) != 1)
            i++;
        while (i < j && ((arr[j] >> bit) & 1) != 0)
            j--;
        if (i < j)
        {
            std::swap(arr[i], arr[j]);
        }
    }
    if (i == j && ((arr[i] >> bit) & 1) == 0)
        i++;
    return i;
}


void msd(uint64_t *arr, size_t L, size_t R, size_t bit)
{
    if (L >= R || bit == -1)
        return;
    size_t i = partition(arr, L, R, bit);
    
    if (i > L)
        msd(arr, L, i - 1, bit - 1);
    if (i < R)
        msd(arr, i, R, bit - 1);
}


int main(int argc, char const *argv[])
{
    size_t n;
    std::cin >> n;
    uint64_t *arr = new uint64_t[n];
    for (size_t i = 0; i < n; i++)
        std::cin >> arr[i];

    msd(arr, 0, n - 1, 63);
    for (size_t i = 0; i < n; i++)
        std::cout << arr[i] << " ";
    std::cout << std::endl;

    delete[] arr;
    return 0;
}
