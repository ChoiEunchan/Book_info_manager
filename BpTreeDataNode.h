#ifndef _BpTreeDataNode_H_
#define _BpTreeDataNode_H_

#include "BpTreeNode.h"

class BpTreeDataNode : public BpTreeNode {
private:
	map <string, LoanBookData*> mapData;
	BpTreeNode* pNext;
	BpTreeNode* pPrev;
public:
	BpTreeDataNode() : BpTreeNode() {
		pNext = nullptr;
		pPrev = nullptr;
	}
	~BpTreeDataNode() {
		if (mapData.size() > 0) {
			map<string, LoanBookData*> :: iterator it_data = mapData.begin();
			delete it_data->second;
			it_data++;
			if (it_data != mapData.end())
				delete it_data->second;
		}
		mapData.clear();
		pNext = nullptr;
		pPrev = nullptr;
		/*	the book information of the elements is first released from memory
			and then the book information map is cleared.	*/
	}

	void setNext(BpTreeNode* pN) { pNext = pN; }
	void setPrev(BpTreeNode* pN) { pPrev = pN; }
	//Update the information of my previous data node and my parent node.
	BpTreeNode* getNext() { return pNext; }
	BpTreeNode* getPrev() { return pPrev; }
	//	Returns the next data node address of the current node and the address of the parent node.
	void insertDataMap(string name, LoanBookData* pN) {
		mapData.insert(map<string, LoanBookData*>::value_type(name, pN));
	}
	//	Add elements to the data map.
	void deleteMap(string name) {
		mapData.erase(name);
	}
	map<string, LoanBookData*>* getDataMap() {
		if (mapData.empty())
			return nullptr;
		else
			return &mapData;
	}
	/*	If there is no book information in the data map, return the nullptr,
		if not, returns the address of the data map.	*/
};

#endif
