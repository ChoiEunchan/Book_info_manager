#include "SelectionTree.h"
#include <map>
#include <stack>
#include <fstream>

bool SelectionTree::Insert(LoanBookData* newData, LoanBookHeap* hRoot) {
	int run_num = newData->getCode()/100, div = 4, position, result;
	SelectionTreeNode* temp = nullptr;
	SelectionTreeNode* it = root;
	string tree_node_key, new_node_key = newData->getName();

	for (int i = 2; i >= 0; i--) {
		if (it != nullptr) {
			tree_node_key = it->getBookData()->getName();
			result = tree_node_key.compare(new_node_key);
			if (result > 0) {
				it->setBookData(newData);
				it->setHeap(hRoot);
			}
		}
		else {
			it = new SelectionTreeNode(newData, hRoot);
			if (temp != nullptr) {
				it->setParent(temp);
				if (position == 0)
					temp->setLeftChild(it);
				else
					temp->setRightChild(it);
			}
			else
				setRoot(it);
		}
		temp = it;
		if (i == 3)
			break;
		position = run_num / div;		
		if (position == 1) {
			it = it->getRightChild();
			div += i;
		}
		else {
			it = it->getLeftChild();
			div -= i;
		}
	}
	if (it == nullptr) {
		it = new SelectionTreeNode(newData, hRoot);
		if (position == 0)
			temp->setLeftChild(it);
		else
			temp->setRightChild(it);
	}
	else {
		tree_node_key = it->getBookData()->getName();
		if (tree_node_key.compare(new_node_key) > 0) {
			it->setBookData(newData);
			it->setHeap(hRoot);
		}
	}
	it->setParent(temp);
	return true;
}

bool SelectionTree::Delete() {
	if (root == nullptr)
		return false;
	
	SelectionTreeNode* it1 = root->getLeftChild();
	SelectionTreeNode* it2 = root->getRightChild();
	SelectionTreeNode* it3 = root;
	LoanBookHeap *drheap = root->getHeap();
	LoanBookHeapNode *drheap_root = drheap->getRoot();
	int result, code = root->getBookData()->getCode()/100, div = 4;
	string str1, str2;
	if (drheap_root != nullptr) {
		drheap->Delete(drheap_root);
		drheap_root = drheap->getRoot();
	}
	for (int i = 2; i >= 0; i--) {
		result = code / div;
		it3->setBookData(nullptr);
		if (result == 0) {
			it3 = it1;
			div -= i;
		}
		else {
			it3 = it2;
			div += i;
		}
		if (i > 0) {
			it1 = it3->getLeftChild();
			it2 = it3->getRightChild();
		}
	}
	//Find the location of run.
	if (drheap_root == nullptr) {
		it3->setBookData(nullptr);
		it1 = it3->getParent();
		if (result == 0)
			it1->setLeftChild(nullptr);
		else
			it1->setRightChild(nullptr);
		delete it3;
		it3 = it1;
	}
	else {
		it3->setBookData(drheap_root->getBookData());
		it3 = it3->getParent();
	}
	it1 = it3->getLeftChild();
	it2 = it3->getRightChild();
	if (it1 == nullptr && it2 == nullptr) {
		delete root;
		root = nullptr;
		return true;
	}
	for (int j = 0; j < 3; j++) {
		if (it1 == nullptr && it2 == nullptr) {
			it1 = it3->getParent();
			if (it1->getLeftChild() == it3)
				it1->setLeftChild(nullptr);
			else
				it1->setRightChild(nullptr);
			delete it3;
			it3 = it1;
		}
		else if ((it1 == nullptr && it2 != nullptr) || (it1 != nullptr && it2 == nullptr)){
			if (it1 != nullptr)
				str1 = it1->getBookData()->getName();
			if (it2 != nullptr)
				str2 = it2->getBookData()->getName();
			if (!str1.empty() && str2.empty()) {
				it3->setBookData(it1->getBookData());
				it3->setRightChild(nullptr);
				delete it2;
			}
			else {
				it3->setBookData(it2->getBookData());
				it3->setLeftChild(nullptr);
				delete it1;
			}
			it3 = it3->getParent();
		}
		else {
			str1 = it1->getBookData()->getName();
			str2 = it2->getBookData()->getName();
			result = str1.compare(str2);
			if (result > 0) {
				it3->setBookData(it2->getBookData());
				it3->setHeap(it2->getHeap());
			}
			else {
				it3->setBookData(it1->getBookData());
				it3->setHeap(it1->getHeap());
			}
			it3 = it3->getParent();
		}
		if (j < 2) {
			it1 = it3->getLeftChild();
			it2 = it3->getRightChild();
		}
	}
	return true;
}

bool SelectionTree::printBookData(int bookCode) {
	int run_num = bookCode/100, div = 4, position;
	SelectionTreeNode* it = root;
	if (it == nullptr || bookCode % 100 > 0)
		return false;
	
	for (int i = 0; i < 3; i++) {
		position = run_num / div;
		if (position == 1) {
			it = it->getRightChild();
			if (i == 0)
				div += 2;
			else
				div += 1;
		}
		else {
			it = it->getLeftChild();
			if (i == 0)
				div -= 2;
			else
				div -= 1;
		}
	}
	if (it == nullptr)
		return false;

	ofstream flog;
	flog.open("log.txt", ios::app);
	flog << "============ PRINT_ST ============" << endl;
	flog.close();
	map <string, LoanBookData*> books_in_heap;
	map <string, LoanBookData*> :: iterator book_it;
	LoanBookHeapNode* node = it->getHeap()->getRoot();
	stack<LoanBookHeapNode*> st;
	pair<string, LoanBookData*> heap_node_data;
	st.push(node);
	while (!st.empty()) {
		node = st.top();
		heap_node_data.first = node->getBookData()->getName();
		heap_node_data.second = node->getBookData();
		st.pop();
		if (node->getLeftChild() != nullptr)
			st.push(node->getLeftChild());
		if (node->getRightChild() != nullptr)
			st.push(node->getRightChild());
		books_in_heap.insert(heap_node_data);
		
	}
	for (book_it = books_in_heap.begin(); book_it != books_in_heap.end(); book_it++)
		book_it->second->printData();
	flog.open("log.txt", ios::app);
	flog << "==================================" << endl << endl;
	flog.close();
	return true;
}