#pragma once
#include <iostream>
#include <fstream>
#include <string>

struct Reader
{
    int num;
    char name[15];
    char address[11];
};

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
    fout.open(name + ".bin", std::ios::out | std::ios::binary);
    while (!fin.eof())
    {
        getline(fin, tmp);
        size++;
    }
    fin.seekg(0);
    Reader readers[size];
    for (int i = 0; i < size; ++i)
    {
        fin >> readers[i].num >> readers[i].name >> readers[i].address;
    }

    for (int i = 0; i < size; ++i)
    {
        fout.write((char*)&readers[i], sizeof(Reader));
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

void print_exact(std::ifstream& fin, std::string name, int tmp)
{
    fin.open(name + ".bin", std::ios::in | std::ios::binary);
    fin.seekg(0, fin.end);
    int size = fin.tellg() / sizeof(Reader);
    fin.seekg(0);
    Reader reader;
    fin.seekg((tmp - 1) * sizeof(Reader));
    std::cout << "Pos: " << tmp - 1 << std::endl;
    fin.read((char*)&reader, sizeof(Reader));
    std::cout << reader.num << " " << reader.name << " " << reader.address << std::endl;
    fin.close();
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
