#include "RBTree.h"

//___HELPERS_____________________________________________________________________________

Node* GetBrother(Node* node)
{
    if (!node)
        return nullptr;
    Node* par = node->parent;
    if (!par)
        return nullptr;
    if (par->left == node)
        return par->right;
    else
        return par->left;
}
Node* GetOtherChild(Node* par, Node* child)
{
    if (!par)
        return nullptr;
    if (par->left == child)
        return par->right;
    else
        return par->left;
}
bool IsRed(Node* node)
{
    return (node && !node->is_black);
}
bool IsBlack(Node* node)
{
    return (!node || node->is_black);
}

void SetLeftChild(Node* child, Node* parent)
{
    if (parent)
        parent->left = child;
    if (child)
        child->parent = parent;
}
void SetRightChild(Node* child, Node* parent)
{
    if (parent)
        parent->right = child;
    if (child)
        child->parent = parent;
}
void SwapChild(Node* child, Node* parent, Node* child_to_swap)
{
    if (parent->left == child_to_swap)
    {
        SetLeftChild(child, parent);
        return;
    }
    if (parent->right == child_to_swap)
        SetRightChild(child, parent);
}

//___TESTS_______________________________________________________________________________

int BlackLen(Node* node)
{
    if (!node)
    {
        return 0;
    }
    int left = BlackLen(node->left);
    int right = BlackLen(node->right);
    if (left != right)
    {
        throw std::runtime_error("Bad len");
    }
    if (node->is_black)
    {
        return left + 1;
    }
    return left;
}

//___BALANCE_____________________________________________________________________________

Node* RBTree::left_rotate(Node* node)
{
    Node* node_r = node->right;
    Node* node_r_l = node_r->left;
    Node* parent = node->parent;

    if (parent)
        SwapChild(node_r, parent, node);
    else
    {
        node_r->parent = nullptr;
        root = node_r;
    }

    SetLeftChild(node, node_r);
    SetRightChild(node_r_l, node);

    return node_r;
}
Node* RBTree::right_rotate(Node* node)
{
    Node* node_l = node->left;
    Node* node_l_r = node_l->right;
    Node* parent = node->parent;

    if (parent)
        SwapChild(node_l, parent, node);
    else
    {
        node_l->parent = nullptr;
        root = node_l;
    }
    SetRightChild(node, node_l);
    SetLeftChild(node_l_r, node);
    return node_l;
}
void RBTree::fixInsert(Node* node)
{
    if (!node || node->is_black)
    {
        return;
    }
    if (node == root)
    {
        node->is_black = true;
        return;
    }

    Node* par = node->parent;
    if (!par || par->is_black)
    {
        return;
    }
    Node* brother = GetBrother(node);

    Node* grandparent = node->parent->parent;
    if (!grandparent)
    {
        return;
    }
    Node* uncle = GetBrother(node->parent);

    if (uncle && !uncle->is_black)
    {
        uncle->is_black = true;
        par->is_black = true;
        grandparent->is_black = false;
        fixInsert(grandparent);
        return;
    }
    if (par == grandparent->left)
    {
        if (node == par->right)
        {
            node = node->parent;
            left_rotate(node);
        }
        node->parent->is_black = true;
        grandparent->is_black = false;
        Node* tmp = right_rotate(grandparent);
        fixInsert(tmp);
    }
    else
    {
        if (node == par->left)
        {
            node = node->parent;
            right_rotate(node);
        }
        node->parent->is_black = true;
        grandparent->is_black = false;
        Node* tmp = left_rotate(grandparent);
        fixInsert(tmp);
    }
}
void RBTree::fixDelete(Node* node, Node* parent, bool deleted_color)
{
    bool node_color = (node ? node->is_black : true);
    if (!node_color || !deleted_color)
    {
        if (node)
        {
            node->is_black = true;
        }
        return;
    }
    while (IsBlack(node) && node != root)
    {
        Node* brother = GetOtherChild(parent, node);
        if (!brother->is_black)
        {
            if (parent->left == node)
            {
                parent = left_rotate(parent);
                parent->is_black = true;
                parent->left->is_black = false;
                parent = parent->left;
            }
            else
            {
                parent = right_rotate(parent);
                parent->is_black = true;
                parent->right->is_black = false;
                parent = parent->right;
            }
        }
        brother = GetOtherChild(parent, node);
        if (!brother->is_black)
        {
            throw std::runtime_error("brother not nigger!");
        }
        if (IsBlack(brother->left) && IsBlack(brother->right))
        {
            bool prev = parent->is_black;
            brother->is_black = false;
            parent->is_black = true;
            try
            {
                BlackLen(parent->parent);
            } catch (...)
            {
                node = parent;
                parent = node->parent;
                continue;
            }
            return;
        }
        if (parent->left == node)
        {
            if (IsBlack(brother->right))
            {
                brother->is_black = false;
                brother->left->is_black = true;
                brother = right_rotate(brother);
            }
            if (IsRed(brother->right))
            {
                brother->is_black = parent->is_black;
                parent->is_black = true;
                brother->right->is_black = true;
                auto tmp = left_rotate(parent);
                return;
            }
        }
        else
        {
            if (IsBlack(brother->left))
            {
                brother->is_black = false;
                brother->right->is_black = true;
                brother = left_rotate(brother);
            }
            if (IsRed(brother->left))
            {
                brother->is_black = parent->is_black;
                parent->is_black = true;
                brother->left->is_black = true;
                auto tmp = right_rotate(parent);
                return;
            }
        }
    }
}