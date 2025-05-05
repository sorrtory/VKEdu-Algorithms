// Федуков Александр WEB-13.
// orlando20056@yandex.ru
//
// Problem:
/*
Реализуйте структуру данных типа “множество строк” на основе динамической хеш-таблицы с открытой адресацией. Хранимые строки непустые и состоят из строчных латинских букв.
Хеш-функция строки должна быть реализована с помощью вычисления значения многочлена методом Горнера. Начальный размер таблицы должен быть равным 8-ми. Перехеширование выполняйте при добавлении элементов в случае, когда коэффициент заполнения таблицы достигает 3/4.
Структура данных должна поддерживать операции добавления строки в множество, удаления строки из множества и проверки принадлежности данной строки множеству.

Вариант 1. Для разрешения коллизий используйте квадратичное пробирование.
i-ая проба g(k, i)=g(k, i-1) + i (mod m). m - степень двойки.
*/

#include <iostream>
#include <vector>
#include "hashtable_tests.h"

const size_t DEFAULT_SIZE = 8;

// Element of the hash table
template <typename T>
struct HashtableNode
{
    HashtableNode()
    {
    }

    HashtableNode(const T &data)
        : data(data), status(2)
    {
    }

    void remove()
    {
        status = 1;
        data = T();
    }

    void set(const T &new_data)
    {
        status = 2;
        data = new_data;
    }

    void copy(const HashtableNode<T> &node)
    {
        status = node.status;
        data = node.data;
    }

    char status = 0; // 0 - empty, 1 - deleted, 2 - occupied
    T data = T();    // Value of the element
};

// Counts hash for strings
class StringHasher
{
public:
    StringHasher(size_t prime = 71)
        : prime(prime)
    {
    }

    size_t operator()(const std::string &str)
    {
        size_t hash = 0;
        for (int i = 0; i < str.size(); i++)
        {
            hash = hash * prime + str[i];
        }
        return hash;
    }

private:
    size_t prime;
};

template <typename T, typename Hasher>
class Hashtable
{
public:
    Hashtable(size_t initial_size = DEFAULT_SIZE)
        : size(0), table(initial_size, HashtableNode<T>())
    {
    }

    ~Hashtable()
    {
        // vector will take it all
    }

    bool Add(const T &key)
    {
        // float alpha = size / table.size(); // Must be less than 3/4
        if (4 * size >= 3 * table.size())
            grow();

        // Get index for the key
        long index = probing(key, table, 1); // Should return index of the
                                             // first empty place || (deleted) key occurance

        // if it's an occurance (We have already this key in the table)
        if (index == -1)
            return false;

        table[index].set(key);
        size++;
        return true;
    }

    bool Has(const T &key)
    {
        long index = probing(key, table, 2);
        return index != -1;
    }

    bool Delete(const T &key)
    {
        long index = probing(key, table, 2);
        // No values found
        if (index == -1)
            return false;

        // The element is in the table and not deleted
        table[index].remove();
        size--;
        return true;
    }

private:
    // Doubles the size of the table
    // Rehashes the table
    void grow()
    {
        std::vector<HashtableNode<T>> new_table(table.size() * 2, HashtableNode<T>());
        for (size_t i = 0; i < table.size(); i++)
        {
            // Skip empty and deleted places
            if (table[i].status == 2)
            {
                long index = probing(table[i].data, new_table, 1); // Get new index
                new_table[index].copy(table[i]);                   // Copy the element
                table[i].remove();                                 // Remove old element
            }
        }
        table = new_table;
    }

    // Jump to the index of the key by several hash calcs
    // Return index of the key OR -1 if it's not found
    // mode 1 - add, 2 - has, 3 - delete
    long probing(const T &key, const std::vector<HashtableNode<T>> &table, char mode)
    {
        long hash = hasher(key); // Hash for the first probe
        long deleted_index = -1; // Index of the deleted place
        for (size_t i = 0; i < table.size(); i++)
        {
            // Index for the i-th probe
            long index = hash % table.size();

            if (mode == 1)
            {
                // Add
                if (table[index].status == 0)
                    // Add to empty place
                    return index;
                else if (table[index].status == 1)
                {
                    // Add to deleted place

                    // Wait to ensure we won't hit the same key later
                    if (deleted_index == -1)
                        deleted_index = index;
                }
                else if (table[index].status == 2)
                {
                    // Check occupied place
                    if (table[index].data == key)
                        // Can't add. Key is already in the table
                        return -1;
                }
            }
            else
            {
                // Has or delete
                if (table[index].status == 0)
                    // Empty place by hash
                    return -1;
                else if (table[index].data == key)
                    // Found the key
                    if (table[index].status == 1)
                        // Deleted
                        return -1;
                    else
                        // Found occurance
                        return index;
            }

            // Hash for (i+1)-th probe for key
            // g(k, i) = g(k, i-1) + i (mod m)
            hash = (hash + i + 1) % table.size();
        }

        if (deleted_index != -1)
            // Found deleted place
            return deleted_index;

        // No value found
        // No empty place
        return -1;
    }

    std::vector<HashtableNode<T>> table; // Places for elements
    size_t size;                         // Number of elements
    Hasher hasher;                       // Hash function
};

int main(int argc, const char *argv[])
{
    run_all_tests<Hashtable<std::string, StringHasher>>();
    return 0;

    Hashtable<std::string, StringHasher> table;

    char op;
    std::string key;

    while (std::cin >> op >> key)
    {
        switch (op)
        {
        case '?':
        {
            std::cout << (table.Has(key) ? "OK" : "FAIL") << std::endl;
            break;
        }
        case '+':
        {
            std::cout << (table.Add(key) ? "OK" : "FAIL") << std::endl;
            break;
        }
        case '-':
        {
            std::cout << (table.Delete(key) ? "OK" : "FAIL") << std::endl;
            break;
        }
        case '!':
        {
            return 0;
        }
        }
    }
    return 0;
}
