#pragma once
#include "Tree.h"

#include <iostream>
#include <iomanip>

class RBTree : public Tree
{
public:
    RBTree() = default;

    // Balance
    Node* left_rotate(Node* node);
    Node* right_rotate(Node* node);

    void fixInsert(Node* node) override;
    void fixDelete(Node* node, Node* parent, bool deleted_color) override;

    // Test
    bool CheckBlackLen();
    bool CheckInvariant();
};