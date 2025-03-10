#pragma once
#include "LoanBookData.h"
#include <map>

class BpTreeNode {
private:
	BpTreeNode* pParent;
	BpTreeNode* pMostLeftChild;

public:
	BpTreeNode() {
		pParent = nullptr;
		pMostLeftChild = nullptr;
	}
	virtual ~BpTreeNode() {
		if (pParent == nullptr) {
			pMostLeftChild = nullptr;
			return;
		}
		map<string, BpTreeNode*>* indexMap = pParent->getIndexMap();
		BpTreeNode* temp = indexMap->begin()->second;
		if (temp == this) 
			temp = nullptr;
		else {
			temp++;
			if (temp == this) 
				temp = nullptr;
			else 
				pParent->getMostLeftChild()->setMostLeftChild(nullptr);
		}
		pParent = nullptr;
		return;
	}

	void setMostLeftChild(BpTreeNode* pN) { pMostLeftChild = pN; }
	void setParent(BpTreeNode* pN) { pParent = pN; }

	BpTreeNode* getParent() { return pParent; }
	BpTreeNode* getMostLeftChild() { return pMostLeftChild; }

	virtual void setNext(BpTreeNode* pN) {}
	virtual void setPrev(BpTreeNode* pN) {}
	virtual BpTreeNode* getNext() { return nullptr; }
	virtual BpTreeNode* getPrev() { return nullptr; }

	virtual void insertDataMap(string n, LoanBookData* pN) {}
	virtual void insertIndexMap(string n, BpTreeNode* pN) {}
	virtual void deleteMap(string n) {}

	virtual map<string, BpTreeNode*>* getIndexMap() { return {}; }
	virtual map<string, LoanBookData*>* getDataMap() { return {}; }

};
