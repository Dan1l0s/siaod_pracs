#include "binary.h"

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

void txt_to_bin(std::ifstream& fin, std::ofstream& fout, std::string name)
{
    std::string tmp;
    int size = 0;
    fin.open("asciii.txt", std::ios::in);
    fin.seekg(0);
    fout.open(name, std::ios::out | std::ios::binary);
    Reader reader;
    while (!fin.eof())
    {
        fin >> reader.num >> reader.name >> reader.address;
        fout.write((char*)&reader, sizeof(Reader));
    }

    if (!fout.good())
    {
        std::cout << "output error";
    }
    fin.close();
    fout.close();
}

void bin_to_txt(std::ifstream& fin, std::ofstream& fout, std::string name)
{
    fout.open(name + ".txt", std::ios::out);
    fin.open(name + ".bin", std::ios::in | std::ios::binary);
    fin.seekg(0, fin.end);
    int size = fin.tellg() / sizeof(Reader);
    fin.seekg(0);
    Reader readers[size];
    for (int i = 0; i < size; ++i)
    {
        fin.read((char*)&readers[i], sizeof(Reader));
    }
    for (int i = 0; i < size; ++i)
    {
        fout << readers[i].num << " " << readers[i].name << " " << readers[i].address << std::endl;
    }
    if (!fout.good())
    {
        std::cout << "output error";
    }
    fin.close();
    fout.close();
}

void add_to_bin(std::ofstream& fout, std::string name)
{
    fout.open(name + ".bin", std::ios::app);

    Reader reader;
    std::cout << "Input reader data: ";
    std::cin >> reader.num >> reader.name >> reader.address;
    fout.write((char*)&reader, sizeof(Reader));

    if (!fout.good())
    {
        std::cout << "output error";
    }
    fout.close();
}

void print_bin(std::ifstream& fin, std::string name)
{
    fin.open(name + ".bin", std::ios::in | std::ios::binary);
    fin.seekg(0, fin.end);
    int size = fin.tellg() / sizeof(Reader);
    fin.seekg(0);
    Reader readers[size];
    for (int i = 0; i < size; ++i)
    {
        fin.read((char*)&readers[i], sizeof(Reader));
    }
    for (int i = 0; i < size; ++i)
    {
        std::cout << readers[i].num << " " << readers[i].name << " " << readers[i].address << std::endl;
    }
    fin.close();
}

void print_exact(std::ifstream& fin, std::string name, int position)
{
    fin.open(name + ".bin", std::ios::in | std::ios::binary);
    fin.seekg(0, fin.end);
    int size = fin.tellg() / sizeof(Reader);
    fin.seekg(0);
    int tmp;
    Reader reader;
    if (position == -1)
    {
        std::cout << "Input number: ";
        while (true)
        {
            std::cin >> tmp;
            if (tmp > size || tmp < 1)
            {
                std::cout << "There's no such number, try again (1-" << size << ")" << std::endl;
            }
            else
            {
                break;
            }
        }
    }
    else
        tmp = position + 1;
    fin.seekg((tmp - 1) * sizeof(Reader));
    fin.read((char*)&reader, sizeof(Reader));
    std::cout << reader.num << " " << reader.name << " " << reader.address << std::endl;
    fin.close();
}

void print_key(std::ifstream& fin, std::string name, int key)
{
    fin.open(name + ".bin", std::ios::in | std::ios::binary);
    fin.seekg(0, fin.end);
    int size = fin.tellg() / sizeof(Reader);
    fin.seekg(0);
    int tmp;
    Reader reader;
    for (int i = 0; i < size; ++i)
    {
        fin.read((char*)&reader, sizeof(Reader));
        if (reader.num == key)
            break;
    }
    fin.close();
    if (reader.num == key)
    {
        std::cout << reader.num << " " << reader.name << " " << reader.address << std::endl;
    }
    else
    {
        std::cout << "there is no such number" << std::endl;
    }
}

void replace_key(std::ifstream& fin, std::ofstream& fout, std::string name)
{
    int num;
    std::cout << "Input required number: ";
    while (true)
    {
        std::cin >> num;
        if (num > 99999 || num < 10000)
        {
            std::cout << "There's no such number, try again (10000-99999)" << std::endl;
        }
        else
        {
            break;
        }
    }
    fin.open(name + ".bin", std::ios::in | std::ios::binary);
    fin.seekg(0, fin.end);
    int size = fin.tellg() / sizeof(Reader);
    fin.seekg(0);
    Reader readers[size];
    for (int i = 0; i < size; ++i)
    {
        fin.read((char*)&readers[i], sizeof(Reader));
    }
    bool ff = true;
    for (int i = 0; i < size; ++i)
    {
        if (readers[i].num == num)
        {
            readers[i] = readers[size - 1];
            size--;
            ff = false;
        }
    }
    if (ff)
    {
        std::cout << "There was no such number\n";
    }
    fin.close();
    fout.open(name + ".bin", std::ios::out | std::ios::binary);
    for (int i = 0; i < size; ++i)
    {
        fout.write((char*)&readers[i], sizeof(Reader));
    }
    if (!fout.good())
    {
        std::cout << "output error\n";
    }
    fout.close();
}

void delete_key(std::ifstream& fin, std::ofstream& fout, std::string name, int num)
{
    if (num == 0)
    {
        std::cout << "Input required number: ";
        while (true)
        {
            std::cin >> num;
            if (num > 99999 || num < 10000)
            {
                std::cout << "There's no such number, try again (10000-99999)" << std::endl;
            }
            else
            {
                break;
            }
        }
    }
    fin.open(name + ".bin", std::ios::in | std::ios::binary);
    fin.seekg(0, fin.end);
    int size = fin.tellg() / sizeof(Reader);
    fin.seekg(0);
    Reader readers[size];
    for (int i = 0; i < size; ++i)
    {
        fin.read((char*)&readers[i], sizeof(Reader));
    }
    bool ff = true;
    for (int i = 0; i < size; ++i)
    {
        if (readers[i].num == num)
        {
            for (int k = i; k < size - 1; ++k)
            {
                readers[k] = readers[k + 1];
            }
            size--;
            ff = false;
            break;
        }
    }
    if (ff)
    {
        std::cout << "There was no such number\n";
    }
    fin.close();
    fout.open(name + ".bin", std::ios::out | std::ios::binary);
    for (int i = 0; i < size; ++i)
    {
        fout.write((char*)&readers[i], sizeof(Reader));
    }
    if (!fout.good())
    {
        std::cout << "output error\n";
    }
    fout.close();
}