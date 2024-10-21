#pragma once
#include "LoanBookData.h"
#include "LoanBookHeap.h"

class SelectionTreeNode {
private:
    LoanBookData* pData;
    SelectionTreeNode* pLeft;
    SelectionTreeNode* pRight;
    SelectionTreeNode* pParent;
    LoanBookHeap* hRoot;

public:
    SelectionTreeNode() {
        this->pData = nullptr;
        this->pLeft = nullptr;
        this->pRight = nullptr;
        this->pParent = nullptr;
        this->hRoot = nullptr;
    }
    SelectionTreeNode(LoanBookData* pData) {
        this->pData = pData;
        this->pLeft = nullptr;
        this->pRight = nullptr;
        this->pParent = nullptr;
        this->hRoot = nullptr;
    }
    SelectionTreeNode(LoanBookData* pData, LoanBookHeap* hRoot) {
        this->pData = pData;
        this->pLeft = nullptr;
        this->pRight = nullptr;
        this->pParent = nullptr;
        this->hRoot = hRoot;
    }
    ~SelectionTreeNode() {
        pLeft = nullptr;
        pRight = nullptr;
        pParent = nullptr;
        hRoot = nullptr;
        if (pData != nullptr)
            delete pData;
    }
    void setBookData(LoanBookData* data) { this->pData = data; }
    void setLeftChild(SelectionTreeNode* pL) { this->pLeft = pL; }
    void setRightChild(SelectionTreeNode* pR) { this->pRight = pR; }
    void setParent(SelectionTreeNode* pP) { this->pParent = pP; }
    void setHeap(LoanBookHeap* pHR) { this->hRoot = pHR; }

    // function to output heap data stored in LoanBookHeap according to book classification code
    LoanBookHeapNode* deepCopy(LoanBookHeapNode* root) {
        if (root == nullptr) return nullptr;
        LoanBookHeapNode* copy = new LoanBookHeapNode();
        copy->setLeftChild(deepCopy(root->getLeftChild()));
        copy->setBookData(root->getBookData());
        copy->setRightChild(deepCopy(root->getRightChild()));
        return copy;
    }
    LoanBookData* getBookData() { return pData; }
    SelectionTreeNode* getLeftChild() { return pLeft; }
    SelectionTreeNode* getRightChild() { return pRight; }
    SelectionTreeNode* getParent() { return pParent; }
    LoanBookHeap* getHeap() { return hRoot; }
};