#include <iostream>
#include <fstream>
#include <string>
#include "hash_binary.h"
#include <chrono>

int main()
{
    std::srand(time(NULL));
    int oper_type = -1;
    std::string name_bin;
    std::cout << "Input bin file name:\n";
    std::getline(std::cin, name_bin);
    std::ofstream fout;
    std::ifstream fin;
    Hash_table table;

    while (true)
    {
        std::cout << "Choose your operation:\n1) Convert txt to bin file\n2) Create table from binary file\n3) Delete note from binary file\n4) Find "
                     "note in binary file\n5) Print hash table\n6) Test hash table \n7) Test hash_function \n0) Exit program\nYour choice is: ";
        std::cin >> oper_type;
        // system("cls");
        switch (oper_type)
        {
            case 0: std::cout << "Exit completed"; return 0;
            case 1: {
                txt_to_bin(fin, fout, name_bin);
                break;
            }
            case 2: {
                create_from_file(fin, name_bin, table);
                break;
            }
            case 3: {
                delete_from_file(fin, fout, name_bin, table);
                break;
            }
            case 4: {
                auto start = std::chrono::high_resolution_clock::now();
                find_in_file(fin, name_bin, table);
                auto end = std::chrono::high_resolution_clock::now();
                std::cout << "It took: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() / double(1000) << " ms"
                          << std::endl;
                break;
            }
            case 5: {
                print_table(table);
                break;
            }
            case 6: {
                test_h();
                break;
            }
            case 7: {
                test_h_function();
                break;
            }
            default: {
                std::cin.clear();
                std::cout << "Incorrect input, please input 0-5 number\n";
            }
        }
    }
}