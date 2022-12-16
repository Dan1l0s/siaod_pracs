#pragma once

#include <iostream>
#include <fstream>
#include <string>

struct Reader
{
    int num;
    char name[15];
    char address[10];
};

bool check_file(std::ofstream& fout, std::string name);

void txt_to_bin(std::ifstream& fin, std::ofstream& fout, std::string name);

void bin_to_txt(std::ifstream& fin, std::ofstream& fout, std::string name);

void add_to_bin(std::ofstream& fout, std::string name);

void print_bin(std::ifstream& fin, std::string name);

void print_exact(std::ifstream& fin, std::string name, int position = -1);

void print_key(std::ifstream& fin, std::string name, int key);

void replace_key(std::ifstream& fin, std::ofstream& fout, std::string name);

void delete_key(std::ifstream& fin, std::ofstream& fout, std::string name, int num = 0);