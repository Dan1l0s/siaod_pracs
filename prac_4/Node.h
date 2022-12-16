#pragma once

#include <iostream>

class Node
{
private:
    int value;
    Node* left = nullptr;
    Node* right = nullptr;

public:
    void create_tree(Node*& root, int n);

    void print_tree(int height, int depth = 0);
    void print_tree_nice();

    int find_height(int depth = 1);
    int find_length(bool first = true);
    double average(int& sum, int& amount, bool first = true);
};