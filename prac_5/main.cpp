#include "RBTree.h"
#include "Tree.h"
#include "binary.h"

#include <map>

int main()
{
    srand(time(NULL));
    int key, position;
    RBTree rb_tree;
    std::ofstream fout;
    std::ifstream fin;
    // Test();
    int oper_type = -1;
    std::string name;
    std::cout << "Input bin file name:\n";
    std::getline(std::cin, name);
    while (true)
    {
        std::cout << "Choose your operation:\n1) Create bin from txt\n2) Print bin file\n3) Add reader to bin file\n4) Print exact reader\n5) "
                     "Print reader at custom pos\n6) Create tree from file \n7) Add reader to tree\n8) Find reader in tree\n9) Delete reader from "
                     "tree\n10) Print tree\n0) Exit program\nYour choice is: ";
        std::cin >> oper_type;
        // system("cls");
        switch (oper_type)
        {
            case 0: std::cout << "Exit completed"; return 0;
            case 1: {
                txt_to_bin(fin, fout, name + ".bin");
                break;
            }
            case 2: {
                if (check_file(fout, name + ".bin"))
                    print_bin(fin, name);
                break;
            }
            case 3: {
                if (check_file(fout, name + ".bin"))
                    add_to_bin(fout, name);
                break;
            }
            case 4: {
                do
                {
                    std::cout << "Input key: ";
                    std::cin >> key;
                    if (key > 9999 && key < 100000)
                        break;
                    std::cout << "Incorrect, input 5-digit key [10000 - 99999]" << std::endl;
                }
                while (true);
                if (check_file(fout, name + ".bin"))
                    print_key(fin, name, key);
                break;
            }
            case 5: {
                if (check_file(fout, name + ".bin"))
                    print_exact(fin, name);
                break;
            }
            case 6: {
                if (check_file(fout, name + ".bin"))
                    rb_tree.create_from_file(fin, name);
                break;
            }
            case 7: {
                if (check_file(fout, name + ".bin"))
                    rb_tree.add_from_file(fin, name);
                break;
            }
            case 8: {
                do
                {
                    std::cout << "Input key: ";
                    std::cin >> key;
                    if (key > 9999 && key < 100000)
                        break;
                    std::cout << "Incorrect, input 5-digit key [10000 - 99999]" << std::endl;
                }
                while (true);
                if (rb_tree[key] == -1)
                    std::cout << "There's no such key in the .bin file" << std::endl;
                else
                {
                    std::cout << "The key is at " << rb_tree[key] << " pos" << std::endl;
                    print_exact(fin, name, rb_tree[key]);
                }
                break;
            }
            case 9: {
                while (true)
                {
                    std::cout << "Input key: ";
                    std::cin >> key;
                    if (key > 9999 && key < 100000)
                        break;
                    std::cout << "Incorrect, input 5-digit key [10000 - 99999]" << std::endl;
                }
                position = rb_tree[key];
                rb_tree.remove(key);

                for (auto& it : rb_tree)
                {
                    if (it.second > position)
                        it.second--;
                }

                delete_key(fin, fout, name, key);
                break;
            }
            case 10: {
                rb_tree.print_tree_nice();
                break;
            }
            default: {
                std::cout << "Incorrect input, please input 0-10 number\n";
            }
        }
    }
    return 0;
    // for (int i = 0; i < 10; ++i)
    // {
    //     rb_tree[rand() % 89999 + 10000] = rand() % 89999 + 10000;
    //     tree[rand() % 89999 + 10000] = rand() % 89999 + 10000;
    // }
    // tree.Print();
    // rb_tree.print_tree_nice();
    // std::cout << "####################" << std::endl;
    // tree.print_tree_nice();
    // for (auto [key, value] : tree)
    // {
    //     std::cout << key << " " << value << std::endl;
    // }
}
