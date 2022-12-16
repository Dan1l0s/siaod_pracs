#include "Tree.h"

#include "binary.h"

Tree::Tree()
{
}

//___FILE_PART___________________________________________________________________________

void Tree::create_from_file(std::ifstream& fin, std::string name)
{
    fin.open(name + ".bin", std::ios::in | std::ios::binary);
    fin.seekg(0, fin.end);
    int size = fin.tellg() / sizeof(Reader);
    fin.close();
    for (int i = 0; i < size; ++i)
    {
        add_from_file(fin, name);
    }
}
void Tree::add_from_file(std::ifstream& fin, std::string name)
{
    fin.open(name + ".bin", std::ios::in | std::ios::binary);
    Reader reader;

    fin.seekg((this->size()) * sizeof(Reader));
    fin.read((char*)&reader, sizeof(Reader));
    insert(reader.num, this->size());

    fin.close();
}

//___TREE_OPS____________________________________________________________________________

int& Tree::operator[](int key)
{
    Node* curr = find_key(key);
    if (!curr || curr->value.first != key)
    {
        insert(key, -1);
        curr = find_key(key);
    }
    return curr->value.second;
}
Node* Tree::find_key(int key)
{
    if (!root)
        return nullptr;
    Node* tmp = root;
    Node* prev = nullptr;
    while (tmp)
    {
        prev = tmp;
        if (tmp->value.first == key)
            return tmp;
        tmp = (tmp->value.first < key) ? tmp->right : tmp->left;
    }
    return prev;
}
void Tree::insert(int key, int pos)
{
    if (!root)
    {
        root = new Node(key, pos);
        _size++;
        root->is_black = true;
        return;
    }
    Node* parent = find_key(key);
    if (parent->value.first == key)
    {
        std::cout << "already inserted " << key << " at " << pos << std::endl;
        return;
    }
    _size++;
    if (parent->value.first <= key)
    {
        parent->right = new Node(key, pos);
        parent->right->parent = parent;
        fixInsert(parent->right);
    }
    else
    {
        parent->left = new Node(key, pos);
        parent->left->parent = parent;
        fixInsert(parent->left);
    }
}
void Tree::remove(int key)
{
    Node* curr = find_key(key);
    if (!curr || curr->value.first != key)
    {
        std::cout << "There's no such key!\n";
        return;
    }
    if (!curr->left && !curr->right)
    {
        if (curr->parent)
        {
            if (curr->parent->left == curr)
                curr->parent->left = nullptr;
            else
                curr->parent->right = nullptr;
            fixDelete(nullptr, curr->parent, curr->is_black);
        }
        else
            root = nullptr;
        delete curr;
        _size--;
    }
    else if (!curr->right && curr->left || curr->right && !curr->left)
    {
        if (!curr->parent)
        {
            if (curr->right)
                root = curr->right;
            else
                root = curr->left;
            root->parent = nullptr;
            root->is_black = true;
            delete curr;
            _size--;
            return;
        }
        if (curr->right)
        {
            if (curr->parent->left == curr)
                curr->parent->left = curr->right;
            else
                curr->parent->right = curr->right;
            curr->right->parent = curr->parent;
            fixDelete(curr->right, curr->parent, curr->is_black);
        }
        else
        {
            if (curr->parent->left == curr)
                curr->parent->left = curr->left;
            else
                curr->parent->right = curr->left;
            curr->left->parent = curr->parent;
            fixDelete(curr->left, curr->parent, curr->is_black);
        }
        delete curr;
        _size--;
    }
    else
    {
        Node* child = curr->right;
        while (child->left)
        {
            child = child->left;
        }
        int tmp1 = child->value.first;
        curr->value.second = child->value.second;
        remove(child->value.first);
        curr->value.first = tmp1;
    }
}

//___HELPERS_____________________________________________________________________________

Node* next(Node* node)
{
    if (node->right)
    {
        node = node->right;
        while (node->left)
        {
            node = node->left;
        }
    }
    else
    {
        while (node->parent && node->parent->left != node)
        {
            node = node->parent;
        }
        if (!node)
            return nullptr;
        node = node->parent;
    }
    return node;
}

//___PRINTS______________________________________________________________________________

void Tree::print_tree(Node* node, int height, int depth)
{
    if (!node)
    {
        if (depth < height)
        {
            std::cout << "|";
            for (int i = 0; i < height; ++i)
            {
                std::cout << "         |";
            }
            std::cout << std::endl;
        }
        return;
    }
    print_tree(node->right, height, depth + 1);
    std::cout << "|";
    for (int i = 0; i < depth; ++i)
    {
        std::cout << "         |";
    }
    std::cout << node->value.first << " " << node->value.second << (node->is_black ? " b" : " r") << "|";
    if (depth < height - 1)
        std::cout << "---------|";
    for (int i = depth + 2; i < height; ++i)
    {
        std::cout << "=========|";
    }
    std::cout << std::endl;
    print_tree(node->left, height, depth + 1);
}
void Tree::print_tree_nice()
{
    int height = find_height(root);

    for (int i = 0; i < height; ++i)
    {
        std::cout << "==========";
    }
    std::cout << "=" << std::endl;

    print_tree(root, height);

    for (int i = 0; i < height; ++i)
    {
        std::cout << "==========";
    }
    std::cout << "=" << std::endl;
}
int Tree::find_height(Node* node)
{
    if (!node)
    {
        return 0;
    }
    int right = find_height(node->right);
    int left = find_height(node->left);
    return std::max(right, left) + 1;
}