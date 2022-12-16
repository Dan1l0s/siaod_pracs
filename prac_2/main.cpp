#include <iostream>
#include <fstream>
#include <string>
#include "binary.cpp"
#include "text.cpp"

int main()
{
    std::srand(time(NULL));
    int oper_type = -1;
    std::cout << "Input txt file name:\n";
    std::string name_txt, name_bin;
    std::getline(std::cin, name_txt);
    std::cout << "Input bin file name:\n";
    std::getline(std::cin, name_bin);
    std::ofstream fout;
    std::ifstream fin;
    while (true)
    {
        std::cout << "Choose your operation:\n1) Create txt file\n2) Print txt file\n3) Add string to txt file\n4) Delete string from txt file\n5) "
                     "Find number in txt file\n6) Convert txt to bin file\n7) Convert bin to txt file\n8) Print bin file\n9) Print exact reader\n10) "
                     "Delete reader and replace\n11) Delete reader\n0) Exit program\nYour choice is: ";
        std::cin >> oper_type;
        // system("cls");
        switch (oper_type)
        {
            case 0: std::cout << "Exit completed"; return 0;
            case 1: {
                create_file(fout, name_txt);
                break;
            }
            case 2: {
                print_txt(fin, name_txt);
                break;
            }
            case 3: {
                add_string(fout, name_txt);
                break;
            }
            case 4: {
                delete_string(fin, fout, name_txt);
                break;
            }
            case 5: {
                int tmp = find_number(fin, name_txt);
                std::cout << "Number in position is " << tmp << std::endl;
                break;
            }
            case 6: {
                txt_to_bin(fin, fout, name_bin);
                break;
            }
            case 7: {
                if (check_file(fout, name_bin + ".bin"))
                    bin_to_txt(fin, fout, name_bin);
                break;
            }
            case 8: {
                if (check_file(fout, name_bin + ".bin"))
                    print_bin(fin, name_bin);
                break;
            }
            case 9: {
                if (check_file(fout, name_bin + ".bin"))
                    print_exact(fin, name_bin);
                break;
            }
            case 10: {
                if (check_file(fout, name_bin + ".bin"))
                    replace_key(fin, fout, name_bin);
                break;
            }
            case 11: {
                if (check_file(fout, name_bin + ".bin"))
                    delete_key(fin, fout, name_bin);
                break;
            }
            default: {
                std::cout << "Incorrect input, please input 0-11 number\n";
            }
        }
    }
    return 0;
}