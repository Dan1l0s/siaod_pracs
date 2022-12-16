#pragma once
#include "hash.h"
#include "binary.h"

void create_from_file(std::ifstream& fin, std::string name, Hash_table& table)
{
    fin.open(name + ".bin", std::ios::in | std::ios::binary);
    fin.seekg(0, fin.end);
    int size = fin.tellg() / sizeof(Reader);
    fin.seekg(0);
    Reader tmp;
    Hash_element item;

    for (int i = 0; i < size; ++i)
    {
        fin.read((char*)&tmp, sizeof(Reader));
        item.key = tmp.num;
        item.pos = i;
        insert_in_table(item, table);
    }
    fin.close();

    // std::cout << "Inserted, list:" << std::endl;
    // print_table(table);
}

void add_from_file(std::ifstream& fin, std::string name, Hash_table& table)
{
    fin.open(name + ".bin", std::ios::in | std::ios::binary);
    fin.seekg(0, fin.end);
    int size = fin.tellg() / sizeof(Reader);
    fin.seekg(0);
    Reader tmp;
    Hash_element item;
    int pos;
    while (true)
    {
        std::cout << "Input file pos: ";
        std::cin >> pos;
        if (pos < 0 || pos >= size)
        {
            std::cout << "Incorrect, pos should be (0-" << size - 1 << ")\n";
        }
        else
            break;
    }
    for (int i = 0; i <= pos; ++i)
    {
        fin.read((char*)&tmp, sizeof(Reader));
    }
    item.key = tmp.num;
    item.pos = pos;
    insert_in_table(item, table);
    fin.close();
}

void delete_from_file(std::ifstream& fin, std::ofstream& fout, std::string name, Hash_table& table)
{
    Hash_element item;
    int key;
    std::cout << "Input key:\n";
    std::cin >> key;
    int pos = find_key_in_table(key, table);
    if (pos == -1)
        std::cout << "There's no such key\n";
    else
    {
        item = get_element(key, table);
        delete_from_table(key, table);
        for (int i = 0; i < table.max_size; ++i)
        {
            if (!table.list[i].is_open && !table.list[i].is_deleted)
            {
                if (table.list[i].pos > item.pos)
                    table.list[i].pos--;
            }
        }
        delete_key(fin, fout, name, key);
    }
}

void find_in_file(std::ifstream& fin, std::string name, Hash_table& table)
{
    Hash_element item;
    int key;
    std::cout << "Input key:\n";
    std::cin >> key;

    int pos = find_key_in_table(key, table);
    if (pos == -1)
    {
        std::cout << "There is no such key\n";
    }
    else
    {
        item = get_element(key, table);
        print_exact(fin, name, item.pos + 1);
    }
}