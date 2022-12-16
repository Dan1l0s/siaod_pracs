#include "Node.h"

int main()
{
    srand(time(NULL));
    int n;
    int sum = 0, amount = 0;
    Node* root = new Node;
    std::cout << "Input amount of nodes: ";
    std::cin >> n;
    root->create_tree(root, n);
    root->print_tree_nice();
    std::cout << "\n";
    std::cout << "height: " << root->find_height() << std::endl;
    std::cout << "length: " << root->find_length() << std::endl;
    std::cout << "average: " << root->average(sum, amount) << std::endl;
}