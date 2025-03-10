#ifndef _BpTree_H_
#define _BpTree_H_

#include "BpTreeDataNode.h"
#include "BpTreeIndexNode.h"
#include "LoanBookData.h"
#include <fstream>
#include <iostream>
#include <map>
#include <stack>

class BpTree {
private:
	BpTreeNode* root;
	int	order;		// m children
public:
	BpTree(int order) {
		root = nullptr;
		this->order = order;
	}
	~BpTree() {
		delete root;
	}
	int Insert(LoanBookData* newData);
	bool excessDataNode(BpTreeNode* pDataNode);
	bool excessIndexNode(BpTreeNode* pIndexNode);
	void splitDataNode(BpTreeNode* pDataNode);
	void splitIndexNode(BpTreeNode* pIndexNode);
	BpTreeNode* getRoot() { return root; }
	BpTreeNode* searchDataNode(const string name);

	bool searchBook(const string name);
	bool searchRange(const string start, const string end);
	void Delete(const string name);
	
};

#endif
