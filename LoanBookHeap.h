#pragma once
#include "LoanBookData.h"
#include "LoanBookHeapNode.h"

class LoanBookHeap {
private:
    LoanBookHeapNode* root;

public:
    LoanBookHeap() {
        this->root = nullptr;
    }
    ~LoanBookHeap() {
        while (root != nullptr)
            Delete(root);
    }
    
    void setRoot(LoanBookHeapNode* pN) { this->root = pN;}
    LoanBookHeapNode* getRoot() { return root; }

    bool heapifyUp(LoanBookHeapNode* pN);
    LoanBookHeapNode* heapifyDown(LoanBookHeapNode* pN);

    bool Insert(LoanBookData* data);
    int getDepth(LoanBookHeapNode* node);
    void Delete(LoanBookHeapNode* node);
};