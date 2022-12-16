#include "Node.h"

void Node::create_tree(Node*& root, int n)
{
    if (n != 0)
    {
        root = new Node();

        int nl = n / 2;
        int nr = n - nl - 1;

        root->value = (rand() % 98) + 1;

        root->left->create_tree(root->left, nl);
        root->right->create_tree(root->right, nr);
    }
}

void Node::print_tree(int height, int depth)
{
    if (!this)
    {
        if (depth == height - 1)
        {
            std::cout << "|";
            for (int i = 0; i < depth + 1; ++i)
            {
                std::cout << "  |";
            }
            std::cout << std::endl;
        }
        return;
    }
    this->right->print_tree(height, depth + 1);
    std::cout << "|";
    for (int i = 0; i < depth; ++i)
    {
        std::cout << "  |";
    }
    if (this->value < 10)
        std::cout << "0";
    std::cout << this->value << "|";
    if (depth < height - 1)
        std::cout << "--|";
    for (int i = depth + 2; i < height; ++i)
    {
        std::cout << "==|";
    }
    std::cout << std::endl;
    this->left->print_tree(height, depth + 1);
}

int Node::find_height(int depth)
{
    if (!this)
    {
        return depth - 1;
    }
    int right = this->right->find_height(depth + 1);
    int left = this->left->find_height(depth + 1);
    return std::max(right, left);
}

void Node::print_tree_nice()
{
    int height = this->find_height();

    for (int i = 0; i < height; ++i)
    {
        std::cout << "===";
    }
    std::cout << "=" << std::endl;

    this->print_tree(height);

    for (int i = 0; i < height; ++i)
    {
        std::cout << "===";
    }
    std::cout << "=" << std::endl;
}

int Node::find_length(bool first)
{
    if (!this)
        return 0;
    return this->right->find_length(false) + this->left->find_length(false) + (!first ? 1 : 0);
}

double Node::average(int& sum, int& amount, bool first)
{
    if (this)
    {
        sum += this->value;
        amount++;
        this->right->average(sum, amount, false);
        this->left->average(sum, amount, false);
        if (first)
        {
            return double(sum) / amount;
        }
    }
    return 0;
}