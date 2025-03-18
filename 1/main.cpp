// Федуков Александр WEB-13.
// orlando20056@yandex.ru
//
// Problem:
/*
1.3 Если в числе содержится только один бит со значением 1, записать в выходной поток OK. Иначе записать FAIL
*/

#include <iostream>

int main(int argc, char const *argv[])
{
    long n;
    std::cin >> n;

    if (n == 0)
    {
        std::cout << "FAIL";
        return 0;
    }

    while (n > 1)
    {
        if ((n & 1) == 1)
        {
            std::cout << "FAIL";
            return 0;
        }

        n = n >> 1;
    }

    std::cout << "OK";
    return 0;
}