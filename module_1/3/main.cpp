// Федуков Александр WEB-13.
// orlando20056@yandex.ru
//
// Problem:
/*
Реализовать очередь с помощью двух стеков. Использовать стек, реализованный с помощью динамического буфера.

Обрабатывать команды push back и pop front.

Формат ввода
В первой строке количество команд n. n ≤ 1000000.

Каждая команда задаётся как 2 целых числа: a b.

a = 2 - pop front
a = 3 - push back

Если дана команда pop front, то число b - ожидаемое значение. Если команда pop front вызвана для пустой структуры данных, то ожидается “-1”.


Формат вывода
Требуется напечатать YES - если все ожидаемые значения совпали. Иначе, если хотя бы одно ожидание не оправдалось, то напечатать NO.
*/

#include <iostream>

class Stack
{
private:
    int *values;
    size_t cap;
    size_t len;

public:
    Stack(size_t len);
    ~Stack();
    Stack(const Stack &other) = delete;
    Stack &operator=(const Stack &other) = delete;
    void Push(int);
    int Pop();
    bool IsEmpty();
};

Stack::Stack(size_t cap)
{
    values = (int *)malloc(cap * sizeof(int));
    this->cap = cap;
    len = 0;
}

Stack::~Stack()
{
    free(values);
}

void Stack::Push(int val)
{
    if (cap == len)
    {
        cap *= 2;
        values = (int *)realloc(values, sizeof(int) * cap);
    }
    len += 1;
    values[len] = val;
}

int Stack::Pop()
{
    if (!IsEmpty())
    {
        len -= 1;
        return values[len + 1];
    }
    throw(len);
}

bool Stack::IsEmpty()
{
    return len == 0;
}

class Queue
{
private:
    Stack s1 = Stack(2);
    Stack s2 = Stack(2);

public:
    Queue();
    ~Queue();
    Queue(const Queue &other) = delete;
    Queue &operator=(const Queue &other) = delete;
    int pop_front();
    void push_back(int);
};

Queue::Queue()
{
}

Queue::~Queue()
{
    // Stack destructors called automatically
}

int Queue::pop_front()
{
    // Catch the empty stack error
    try
    {
        if (s2.IsEmpty())
        {
            while (!s1.IsEmpty())
                s2.Push(s1.Pop());
        }
        return s2.Pop();
    }

    catch (...)
    {
        return -1;
    }
}

void Queue::push_back(int val)
{
    s1.Push(val);
}

int main(int argc, char const *argv[])
{
    bool OK = true;
    Queue q;

    int n;
    std::cin >> n;
    for (size_t i = 0; i < n && OK; i++)
    {
        int cmd, val;
        std::cin >> cmd >> val;

        switch (cmd)
        {
        case 2:
            OK = q.pop_front() == val;
            break;
        case 3:
            q.push_back(val);
            break;
        default:
            break;
        }
    }

    std::cout << (OK ? "YES" : "NO");
    return 0;
}
