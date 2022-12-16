#include <iostream>
#include <cmath>

using ull = unsigned long long;

ull fib(int n)
{
    if (n == 1 || n == 2)
        return 1;
    return fib(n - 1) + fib(n - 2);
}

ull func(int n)
{
    ull* array = new ull[std::max(n, 3)];
    array[0] = 2;
    array[1] = 3;
    for (int i = 2; i <= n; ++i)
    {
        array[i] = array[i - 1] + array[i - 2];
    }
    return array[n - 1];
}

int main()
{
    int n;
    while (true)
    {
        std::cout << "Input n:\n";
        std::cin >> n;
        if (n < 1)
            std::cout << "Incorrect input, try again\n";
        else
            break;
    }
    ull full = std::pow(2, n);
    std::cout << "Full: " << full << std::endl;
    std::cout << func(n) << std::endl;
    // std::cout << fib(n + 2) << std::endl;
    return 0;
}
