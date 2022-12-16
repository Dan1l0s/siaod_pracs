#include <iostream>
#include <cmath>

#define uint unsigned int

void printBin(uint num, int size)
{
    std::string res = "";
    while (num > 0)
    {
        res = std::to_string(num % 2) + res;
        num /= 2;
    }
    if (res.size() < size)
    {
        int tmp = size - res.size();
        for (int i = 0; i < tmp; i++)
        {
            res = "0" + res;
        }
    }
    std::cout << res << std::endl;
}

void task_1(uint num)
{
    unsigned int mask = 0b10100000;
    std::cout << "Initial: ";
    printBin(num, 8);
    std::cout << "Result:  ";
    printBin(num | mask, 8);
}

void task_2(uint num)
{
    unsigned int mask = 0b1111000000000;
    mask = ~mask;
    std::cout << "The number is " << num << std::endl
              << "Initial: ";
    printBin(num, 16);
    std::cout << "Result:  ";
    printBin(num & mask, 16);
}

void task_3(uint num)
{
    std::cout << "The number is " << num << std::endl
              << "Initial: ";
    printBin(num, 16);
    std::cout << "Result:  ";
    printBin(num << 3, 16);
}

void task_4(uint num)
{
    std::cout << "The number is " << num << std::endl
              << "Initial: ";
    printBin(num, 16);
    std::cout << "Result:  ";
    printBin(num >> 3, 16);
}

void task_5(uint num, int n)
{
    uint mask = 1;
    mask <<= n;
    std::cout << "The number is " << num << std::endl
              << "Initial: ";
    printBin(num, 16);
    std::cout << "Mask:    ";
    printBin(mask, 16);
    std::cout << "Result:  ";
    printBin(num | mask, 16);
}

int main()
{
    uint hex_num, num;
    std::cout << "Input hex number for task 1: ";
    std::cin >> std::hex >> hex_num;
    std::cout << "Input decimal number for tasks 2-5: ";
    std::cin >> std::dec >> num;
    int oper_type = -1;
    while (true)
    {
        std::cout << "Choose your operation:\n1) Set several bits to '1'\n2) Set several bits to '2'\n3) Multiple by 8\n4) Divide by 8\n5) Set custom bit to '1'\n0) Exit program\nYour choice is: ";
        std::cin >> oper_type;
        switch (oper_type)
        {
        case 0:
            std::cout << "Exit completed";
            return 0;
        case 1:
        {
            task_1(hex_num);
            break;
        }
        case 2:
        {
            task_2(num);
            break;
        }
        case 3:
        {
            task_3(num);
            break;
        }
        case 4:
        {
            task_4(num);
            break;
        }
        case 5:
        {
            std::cout << "Input the number of bit to change: ";
            int n;
            std::cin >> n;
            task_5(num, n);
            break;
        }
        default:
        {
            std::cout << "Incorrect input, please input 0-5 digit\n";
        }
        }
    }
    return 0;
}