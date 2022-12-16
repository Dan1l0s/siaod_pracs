#include <iostream>
#include <fstream>
#include <string>

void create_file(std::ofstream& fout, std::string name)
{
    fout.open(name + ".txt", std::ios::out);
    int n = 0;
    std::cout << "Input total amount of numbers: ";
    std::cin >> n;
    int line_size = 0, tmp = 0;
    for (int i = 0; n > 0; ++i)
    {
        if (i > 0)
        {
            fout << '\n';
        }
        line_size = rand() % 4 + 2;
        if (line_size > n)
        {
            line_size = n;
        }
        for (int k = 0; k < line_size; ++k)
        {
            tmp = rand() % 9 + 1;
            (k != line_size - 1 ? fout << tmp << " " : fout << tmp);
        }
        n -= line_size;
    }
    if (!fout.good())
    {
        std::cout << "output error";
    }
    fout.close();
}

void print_txt(std::ifstream& fin, std::string name)
{
    std::string tmp;
    fin.open(name + ".txt", std::ios::in);
    while (!fin.eof())
    {
        std::getline(fin, tmp);
        std::cout << tmp << std::endl;
    }
    if (fin.bad() || fin.fail())
    {
        std::cout << "input fail" << std::endl;
    }
    fin.close();
}

void add_string(std::ofstream& fout, std::string name)
{
    fout.open(name + ".txt", std::ios::app);
    int line_size, tmp;
    fout << "\n";
    line_size = rand() % 4 + 2;
    for (int k = 0; k < line_size; ++k)
    {
        tmp = rand() % 9 + 1;
        (k != line_size - 1 ? fout << tmp << " " : fout << tmp);
    }
    if (!fout.good())
    {
        std::cout << "output error\n";
    }
    fout.close();
}

int count_numbers(std::ifstream& fin, std::string name)
{
    int tmp, cnt = 0;
    fin.open(name + ".txt", std::ios::in);
    while (!fin.eof())
    {
        fin >> tmp;
        cnt++;
    }
    if (fin.bad() || fin.fail())
    {
        std::cout << "Failed to open file" << std::endl;
    }
    fin.close();
    return cnt;
}

int find_number(std::ifstream& fin, std::string name)
{
    int num = -1, pos, cnt = 0;
    int n = count_numbers(fin, name);
    fin.open(name + ".txt", std::ios::in);
    std::cout << "Input number's place: ";
    while (true)
    {
        std::cin >> pos;
        if (pos > n)
        {
            std::cout << "Pos should not be more than " << n << ", try again" << std::endl;
        }
        else
        {
            break;
        }
    }
    while (cnt < pos)
    {
        fin >> num;
        cnt++;
    }
    if (fin.bad() || fin.fail())
    {
        std::cout << "Failed to open file" << std::endl;
    }
    fin.close();
    return num;
}

bool check_file(std::ofstream& fout, std::string name)
{
    bool ff = true;
    fout.open(name, std::ios::in);
    if (!fout.is_open())
    {
        ff = false;
        std::cout << name << " file does not exist" << std::endl;
    }
    if (!fout.good())
    {
        ff = false;
        std::cout << "i/o error" << std::endl;
    }
    fout.close();
    return ff;
}

void delete_string(std::ifstream& fin, std::ofstream& fout, std::string name)
{
    int line;
    std::string tmp;
    std::cout << "Input string number: ";
    std::cin >> line;
    fout.open("tmp.txt", std::ios::out);
    fin.open(name + ".txt", std::ios::in);
    for (int i = 1; !fin.eof(); ++i)
    {
        std::getline(fin, tmp);
        if (i != line)
        {
            fout << tmp;
            if (!fin.eof())
            {
                fout << "\n";
            }
        }
    }
    name += ".txt";
    const char* name1 = name.c_str();
    if (!fout.good())
    {
        std::cout << "output error\n";
    }
    fin.close();
    fout.close();
    remove(name1);
    rename("tmp.txt", name1);
}
