#pragma once
#include <iostream>
#include <iomanip>

struct Hash_element
{
    int key = 0;
    bool is_open = true;
    bool is_deleted = false;
    int pos = -1;
};

struct Hash_table
{
    int curr_size = 0;
    int max_size = 2;
    Hash_element* list = new Hash_element[max_size];
};

void insert_in_table(Hash_element item, Hash_table& table);

int hash_function(int key, int size)
{
    unsigned int hash = 1;
    for (int i = 1; i < 6; ++i)
    {
        hash = hash * (key % 10 + 2) + key % 10 * 3;
        key /= 10;
    }
    return hash % size;
}

void print_table(Hash_table& table)
{
    for (int i = 0; i < table.max_size; ++i)
    {
        std::cout << "index: " << i << " key: " << std::setw(5) << table.list[i].key << " pos: " << table.list[i].pos << " is deleted? "
                  << table.list[i].is_deleted << std::endl;
    }
}

void upscale_table(Hash_table& table)
{
    if (table.max_size - table.curr_size == 1)
    {
        Hash_table new_table;
        delete[] new_table.list;
        new_table.list = new Hash_element[table.max_size * 2];
        new_table.curr_size = 0;
        new_table.max_size = table.max_size * 2;
        for (int i = 0; i < table.max_size; ++i)
        {
            if (!table.list[i].is_open && !table.list[i].is_deleted)
            {
                insert_in_table(table.list[i], new_table);
            }
        }
        delete[] table.list;
        table = new_table;
    }
}

int find_key_in_table(int key, Hash_table& table)
{
    int pos = hash_function(key, table.max_size);
    while (true)
    {
        if (table.list[pos].is_open)
        {
            return -1;
        }
        if (!table.list[pos].is_deleted && table.list[pos].key == key)
        {
            return pos;
        }
        pos++;
        pos %= table.max_size;
    }
}

void insert_in_table(Hash_element item, Hash_table& table)
{
    upscale_table(table);
    if (find_key_in_table(item.key, table) != -1)
    {
        std::cout << "This key is already in the table\n";
    }
    else
    {

        int pos = hash_function(item.key, table.max_size);
        while (!table.list[pos].is_open)
        {
            pos++;
            pos %= table.max_size;
        }

        table.curr_size++;
        table.list[pos] = item;
        table.list[pos].is_open = false;
        table.list[pos].is_deleted = false;
    }
}

Hash_element get_element(int key, Hash_table& table)
{
    return table.list[find_key_in_table(key, table)];
}

void delete_from_table(int key, Hash_table& table)
{
    int pos = find_key_in_table(key, table);
    if (pos == -1)
    {
        std::cout << "There's no such key in table " << std::endl;
    }
    else
    {
        table.curr_size--;
        table.list[pos].is_open = true;
        table.list[pos].is_deleted = true;
        std::cout << "The element on index " << pos << " was successfully deleted" << std::endl;
    }
}

void test_h()
{
    std::srand(time(NULL));
    Hash_table table;
    int keys[10];
    for (int i = 0; i < 10; ++i)
    {
        keys[i] = ((rand() % 90000) + 10000);
        insert_in_table({keys[i], true, false}, table);
    }
    std::cout << "table:" << std::endl;
    for (int i = 0; i < table.max_size; ++i)
    {
        std::cout << table.list[i].key << std::endl;
    }
    std::cout << "key + index:" << std::endl;

    for (int i = 0; i < 10; ++i)
    {
        std::cout << get_element(keys[i], table).key << " " << find_key_in_table(keys[i], table) << std::endl;
    }
    delete[] table.list;
}

void test_h_function()
{
    std::srand(time(NULL));
    for (int i = 0; i < 10; ++i)
    {
        int tmp = ((rand() % 90000) + 10000);
        std::cout << tmp << " " << hash_function(tmp, 20) << std::endl;
    }
}