#pragma once

#include <iostream>
#include <iomanip>

struct Node
{
    std::pair<int, int> value;  // key and pos
    bool is_black = false;

    Node* left = nullptr;
    Node* right = nullptr;
    Node* parent = nullptr;

    Node(int key, int pos)
    {
        value.first = key;
        value.second = pos;
    }
};

void PrintPair(Node* node);
Node* next(Node* node);

class Tree
{
protected:
    int _size = 0;
    Node* root = nullptr;

public:
    Tree();
    virtual ~Tree(){};

    // RB_part
    virtual void fixInsert(Node* node){};
    virtual void fixDelete(Node* node, Node* parent, bool deleted_color){};

    // file_part
    void add_from_file(std::ifstream& fin, std::string name);
    void create_from_file(std::ifstream& fin, std::string name);

    // tree_ops
    void insert(int key, int pos);
    void remove(int key);
    Node* find_key(int key);
    int& operator[](int key);
    int size()
    {
        return _size;
    }

    // print
    void print_tree_nice();
    void print_tree(Node* node, int height, int depth = 0);
    int find_height(Node* node);

    // iterator
    class TreeIterator
    {
        Node* ptr;

    public:
        TreeIterator(Node* ptr)
        {
            this->ptr = ptr;
        }
        TreeIterator& operator++()
        {
            ptr = next(ptr);
            return *this;
        }
        bool operator==(const TreeIterator& other) const
        {
            return ptr == other.ptr;
        }
        std::pair<int, int>& operator*()
        {
            return ptr->value;
        }
        std::pair<int, int>* operator->()
        {
            return &ptr->value;
        }
        bool operator!=(const TreeIterator& other) const
        {
            return ptr != other.ptr;
        }
    };

    TreeIterator begin()
    {
        Node* node = root;
        if (root)
            while (node->left)
                node = node->left;
        return TreeIterator(node);
    }
    TreeIterator end()
    {
        return TreeIterator(nullptr);
    }
};