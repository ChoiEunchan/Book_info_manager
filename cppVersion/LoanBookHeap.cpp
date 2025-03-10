#pragma once
#include "LoanBookHeap.h"
#include <string>
#include <stack>

bool LoanBookHeap::heapifyUp(LoanBookHeapNode* pN) {
    LoanBookHeapNode* ppN = pN->getParent();
    string parent_key = ppN->getBookData()->getName();
    string node_key = pN->getBookData()->getName();
    int result = node_key.compare(parent_key);
    if (result < 0) {
        LoanBookHeapNode* tempR = pN->getRightChild();
        LoanBookHeapNode* tempL = pN->getLeftChild();
        LoanBookHeapNode* temp;
        if (root == ppN)
            setRoot(pN);
        else {
            if (ppN == ppN->getParent()->getLeftChild())
                ppN->getParent()->setLeftChild(pN);
            else
                ppN->getParent()->setRightChild(pN);
        }
        pN->setParent(ppN->getParent());
        ppN->setParent(pN);       
        if (ppN->getLeftChild() == pN) {
            temp = ppN->getRightChild();
            pN->setLeftChild(ppN);
            pN->setRightChild(temp);
            if (temp != nullptr)
                temp->setParent(pN);
        }
        else {
            temp = ppN->getLeftChild();
            pN->setLeftChild(temp);
            pN->setRightChild(ppN);
            if (temp != nullptr)
                temp->setParent(pN);
        }
        ppN->setLeftChild(tempL);
        if (tempL != nullptr)
            tempL->setParent(ppN);

        ppN->setRightChild(tempR);
        if (tempR != nullptr)
            tempR->setParent(ppN);
        return false;
    }
    else
        return true;
}

LoanBookHeapNode* LoanBookHeap::heapifyDown(LoanBookHeapNode* pN) {
    LoanBookHeapNode* left_child = pN->getLeftChild();
    LoanBookHeapNode* right_child = pN->getRightChild();
    if (left_child == nullptr && right_child == nullptr)
        return nullptr;
    LoanBookHeapNode* it = nullptr;
    LoanBookData *temp;
    LoanBookData *org_book_data = pN->getBookData();
    LoanBookData *new_book_data;
    string str1, str2;
    int code, year, loan_count, result = 0;
    if (left_child != nullptr)
        str1 = left_child->getBookData()->getName();
    if (right_child != nullptr)
        str2 = right_child->getBookData()->getName();

    if (!str1.empty() && str2.empty())
        it = left_child;
    else if (str1.empty() && !str2.empty())
        it = right_child;
    else {
        result = str1.compare(str2);
        if (result > 0)
            it = right_child;
        else
            it = left_child;
    }
    new_book_data = it->getBookData();
    pN->setBookData(new_book_data);
    it->setBookData(org_book_data);
    return it;
}

bool LoanBookHeap::Insert(LoanBookData* data) {
    if (root == nullptr) {
        root = new LoanBookHeapNode(data);
        return true;
    }
    else {
        LoanBookHeapNode* it = root;
        LoanBookHeapNode* itp;
        int direction = 0, ldepth = 0, rdepth = 0;
        
        while (it != nullptr) {
            itp = it;
            ldepth = getDepth(it->getLeftChild());
            rdepth = getDepth(it->getRightChild());
            if (ldepth < rdepth) {
                it = it->getLeftChild();
                direction = -1;
            }
            else if (ldepth > rdepth) {
                it = it->getRightChild();
                direction = 1;
            }
            else {
                it = itp->getLeftChild();
                if (it == nullptr) {
                    it = itp->getLeftChild();
                    direction = -1;
                    continue;
                }
                ldepth = 0;
                rdepth = 0;
                if (it->getLeftChild() != nullptr)
                    ldepth++;
                if (it->getRightChild() != nullptr)
                    ldepth++;
                it = itp->getRightChild();
                if (it->getLeftChild() != nullptr)
                    rdepth++;
                if (it->getRightChild() != nullptr)
                    rdepth++;
                if (ldepth <= rdepth) {
                    it = itp->getLeftChild();
                    direction = -1;
                }
                else {
                    it = itp->getRightChild();
                    direction = 1;
                }
            }
        }
        it = new LoanBookHeapNode(data);
        it->setParent(itp);
        if (direction == 1)
            itp->setRightChild(it);
        else if (direction == -1)
            itp->setLeftChild(it);
        while (root != it) {
            if (heapifyUp(it))
                break;
        }
        return true;
    }
}

int LoanBookHeap::getDepth(LoanBookHeapNode* node) {
    int depth = 0;
    while (node != nullptr) {
        depth++;
        node = node->getLeftChild();
    }
    return depth;
}

void LoanBookHeap::Delete(LoanBookHeapNode* node) {
    LoanBookHeapNode *it;
    LoanBookHeapNode* temp;
    string node1_key, node2_key;
    int result = 0;
    stack <LoanBookHeapNode*> st;
    node1_key = node->getBookData()->getName();
    st.push(root);
    while (!st.empty()) {
        it = st.top();
        node2_key = it->getBookData()->getName();
        result = node2_key.compare(node1_key);
        if (result != 0) {
            st.pop();
            if (it->getLeftChild() != nullptr) {
                it = it->getLeftChild();
                st.push(it);
            }        
            if (it->getLeftChild() != nullptr) {
                it = it->getRightChild();
                st.push(it);
            }
        }
        else
            break;
    }

    if (st.empty())
        return;
    
    while (it != nullptr) {
        temp = heapifyDown(it);
        if (temp == nullptr)
            break;
        it = temp;
    }
    temp = it->getParent();
    if (temp != nullptr) {
        if (temp->getLeftChild() == it)
            temp->setLeftChild(nullptr);
        else
            temp->setRightChild(nullptr);
    }
    else {
        root = nullptr;
    }
    delete it;
    return;
}