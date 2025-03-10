#pragma once
#include "SelectionTreeNode.h"

#include <fstream>
#include <iostream>

class SelectionTree
{
private:
    SelectionTreeNode* root;

public:
    SelectionTree() {
        this->root = nullptr;
    }
    ~SelectionTree() {
        while (root != nullptr)
            Delete();
    }

    void setRoot(SelectionTreeNode* pN) { this->root = pN; }
    SelectionTreeNode* getRoot() { return root; }

    bool Insert(LoanBookData* newData, LoanBookHeap* hRoot);
    bool Delete();
    bool printBookData(int bookCode);
};