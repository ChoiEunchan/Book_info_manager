#ifndef _BPTREEINDEXNODE_H_
#define _BPTREEINDEXNODE_H_

#include "BpTreeNode.h"
#include <map>

class BpTreeIndexNode : public BpTreeNode {
private:
	map <string, BpTreeNode*> mapIndex;

public:
	BpTreeIndexNode() : BpTreeNode() {	}
	~BpTreeIndexNode() {
		if (getMostLeftChild() != nullptr)
			delete getMostLeftChild();
		if (mapIndex.size() >0) {
			map<string, BpTreeNode*> :: iterator it = mapIndex.begin();
			delete it->second;
			it++;
			if (it != mapIndex.end()) {
				delete it->second;
			}
			
		}
		mapIndex.clear();
		map<string, BpTreeNode*>().swap(mapIndex);
	}

	void insertIndexMap(string name, BpTreeNode* pN) {
		mapIndex.insert(map<string, BpTreeNode*>::value_type(name, pN));
	}

	void deleteMap(string name) {
		mapIndex.erase(name);
	}

	map <string, BpTreeNode*>* getIndexMap() {
		if (mapIndex.empty())
			return nullptr;
		else
			return &mapIndex;
	}

};

#endif