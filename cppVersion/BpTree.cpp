#include "BpTree.h"
#include <vector>
#include <fstream>

int BpTree::Insert(LoanBookData* newData) {
	int updated_loan_count = 0, code = 0;
	if (root != nullptr) {
	//If there's data in the tree, then execute the following code.
		string newData_key = newData->getName();
		BpTreeNode* it = searchDataNode(newData_key);
		BpTreeNode* it_parent;
		map <string, LoanBookData*>* it_data_map;
		map <string, LoanBookData*> :: iterator data_map_it;
		if (it->getDataMap() != nullptr) {
			it_parent = it->getParent();
			it_data_map = it->getDataMap();
			data_map_it = it_data_map->find(newData_key);
			if (data_map_it != it_data_map->end()) {
				data_map_it->second->updateCount();
				code = -1;
				updated_loan_count = data_map_it->second->getLoanCount();
			}
			else {
				it->insertDataMap(newData_key, newData);
				updated_loan_count = newData->getLoanCount();
				if (updated_loan_count > 0)
					code = -1;
			}
		}
		else {
			it_parent = it;
			it = new BpTreeDataNode();
			it->insertDataMap(newData_key, newData);
			it->setParent(it_parent);
			it_parent->setMostLeftChild(it);
			/*	If the "it" is the most left child of its parent, and it is nullptr,
				set "it" as parent's most left child.	*/
			BpTreeNode* temp = it_parent->getIndexMap()->begin()->second;
			it->setNext(temp);
			temp->setPrev(it);
			if (temp->getPrev() != nullptr && temp->getPrev() != it) {
				temp->getPrev()->setNext(it);
				it->setPrev(temp->getPrev());
			}
			// Connect new data node and other data node.
			updated_loan_count = newData->getLoanCount();
			if (updated_loan_count > 0)
				code = -1;
		}
		if (excessDataNode(it))
			splitDataNode(it);		
		//If the node has three elements, split the node and returns true.
	}
	else {
			root = new BpTreeIndexNode();
			BpTreeNode *root_data = new BpTreeDataNode();
			root->insertIndexMap(newData->getName(), root_data);
			root_data->insertDataMap(newData->getName(), newData);
			root_data->setParent(root);
			updated_loan_count = newData->getLoanCount();
			if (updated_loan_count > 0)
				code = -1;
		//	If there's no data in the tree, make new root index node and data node and connect them.
	}
	
	if (code == -1) {
		code = newData->getCode();
		if (code >= 0 && code <= 200) {
			if (updated_loan_count == 3) code = -2;
		}
		else if (code >= 300 && code <= 400) {
			if (updated_loan_count == 4) code = -2;
		}
		else if (code >= 500 && code <= 700) {
			if (updated_loan_count == 2) code = -2;
		}
	}
	/*  If the node's loan count is same with the number of available loan volumes
		according to its loan count and classification code, set code -2.	*/
	if (code == -2) {
		Delete(newData->getName());
		//If the node's loan count is same with the number of available loan volumes,
		//execute the delete function.
		return -2;
	}
	else
		return updated_loan_count;
}

bool BpTree::excessDataNode(BpTreeNode* pDataNode) {
	if (pDataNode->getDataMap()->size() > order - 1) return true;//order is equal to the number of elements 
	else return false;
}

bool BpTree::excessIndexNode(BpTreeNode* pIndexNode) {
	if (pIndexNode->getIndexMap()->size() > order - 1)return true;//order is equal to the number of elements 
	else return false;
}

void BpTree::splitDataNode(BpTreeNode* pDataNode) {
	map<string, LoanBookData*>* origin_data_map = pDataNode->getDataMap();
	map<string, LoanBookData*>::iterator target = origin_data_map->begin();
	BpTreeNode *parent = pDataNode->getParent();
	BpTreeNode *new_data_node = new BpTreeDataNode();
	target++;
	new_data_node->insertDataMap(target->first, target->second);
	target++;
	new_data_node->insertDataMap(target->first, target->second);
	//	Create a new data node with data other than the one with the smallest key value in the data node.
	origin_data_map->erase(target);
	target = origin_data_map->begin();
	target++;
	origin_data_map->erase(target);
	//	Deletes all map data except for values ​​not copied from the existing data map.
	if (pDataNode->getNext() != nullptr) {
		new_data_node->setNext(pDataNode->getNext());
		pDataNode->getNext()->setPrev(new_data_node);
	}
	pDataNode->setNext(new_data_node);
	new_data_node->setPrev(pDataNode);

	new_data_node->setParent(parent);
	/*	Connect dlivered data node and newly created data node.
		And set parent of new data node	as origin node's parent.	*/
	target = new_data_node->getDataMap()->begin();
	parent->insertIndexMap(target->first, new_data_node);
	//	Insert the newly created index data into the parent index node
	if (excessIndexNode(parent))
		splitIndexNode(parent);
	//	If overflow occurs in the parent index node, executes the splitIndexNode function.
	return;
}

void BpTree::splitIndexNode(BpTreeNode* pIndexNode) {
	map<string, BpTreeNode*>* origin_index_map = pIndexNode->getIndexMap();
	map<string, BpTreeNode*>::iterator target = origin_index_map->end();
	BpTreeNode *parent = pIndexNode->getParent();
	BpTreeNode *new_parent_index_node;
	BpTreeNode *new_child_index_node = new BpTreeIndexNode();
	target--;
	new_child_index_node->insertIndexMap(target->first, target->second);
	target->second->setParent(new_child_index_node);
	target--;
	if (parent != nullptr) {
		parent->insertIndexMap(target->first, new_child_index_node);
		new_child_index_node->setParent(parent);
		/*	If there's a parent node, insert index key and index node data to the parent
			and set the new child node's parent as parent of delivered node.	*/
	}
	else {
		new_parent_index_node = new BpTreeIndexNode();
		new_parent_index_node->insertIndexMap(target->first, new_child_index_node);
		new_parent_index_node->setMostLeftChild(pIndexNode);
		//If the delivered one is root, make new index node and set the index data and its child.
		new_child_index_node->setParent(new_parent_index_node);
		pIndexNode->setParent(new_parent_index_node);
		//Set new parent node as make newly.
		root = new_parent_index_node;
		//Set root as new node.
		parent = pIndexNode->getParent();
	}
	target->second->setParent(new_child_index_node);
	new_child_index_node->setMostLeftChild(target->second);
	//	Set new child node as parent of dlivered node's second biggest key node.
	
	origin_index_map->erase(target);
	target = origin_index_map->begin();
	target++;
	origin_index_map->erase(target);
	//	Deletes all map data except for values ​​not copied from the existing data map.
	if (excessIndexNode(parent))
		splitIndexNode(parent);
	//	If overflow occurs in the parent index node, executes the splitIndexNode function.
	return;
}

BpTreeNode* BpTree::searchDataNode(const string name) {
	BpTreeNode* it = root;
	if (it == nullptr)
		return nullptr;
	BpTreeNode* it_parent;
	int result = 0;
	map <string, BpTreeNode*>* it_index_map;
	map <string, BpTreeNode*> :: iterator map_it;
	while (it != nullptr && (it->getIndexMap() != nullptr)) {
		it_parent = it;
		it_index_map = it->getIndexMap();
		map_it = it_index_map->find(name);
		if (map_it != it_index_map->end())
			it = map_it->second;
		//	If the index node has element with the key delivered, move it to the element's child node.
		else {
			map_it = it_index_map->begin();
			if (it_index_map->size() == 1) {
				result = map_it->first.compare(name);
				if (result > 0)
					it = it->getMostLeftChild();
				else
					it = map_it->second;
			}
			//If not and if the index node has one element key, determine the direction and move.
			else {
				result = map_it->first.compare(name);
				if (result > 0)
					it = it->getMostLeftChild();
				else {
					map_it++;
					result = map_it->first.compare(name);
					if (result > 0)
						map_it--;
					it = map_it->second;
				}
			}
			//Compare the key values ​​of two elements to determine the direction to move
		}
	}
	if (it == nullptr)
		return it_parent;
	else
		return it;
	/*	Passes the address of the data node. If a leaf node is reached, but there is no data
		for the corresponding key value, return the address of the parent node.	*/
}

bool BpTree::searchBook(const string name) {
	BpTreeNode* node = searchDataNode(name);
	//	Find the data node corresponding to the category of the key value.
	map <string, LoanBookData*>* it_data = node->getDataMap();
	map <string, LoanBookData*> :: iterator it;
	it = it_data->find(name);
	if (it == it_data->end())
		return false;
	//	If data corresponding to the key value does not exists in the node, return the error code.
	else {
		ofstream flog;
		flog.open("log.txt", ios::app);
		flog << "============ SEARCH_BP ============" << endl;
        flog.close();
		it->second->printData();
		flog.open("log.txt", ios::app);
        flog << "===================================" << endl << endl;
		return true;
	}
	//	If not, print the book information.
}

bool BpTree::searchRange(const string start, const string end) {
	BpTreeNode* node = searchDataNode(start);
	BpTreeNode* parent = node->getParent();
	
	if (node == nullptr)
		return false;
	//If there's no data in the data node, return the error code.
	map <string, LoanBookData*>* node_data = node->getDataMap();
	map <string, LoanBookData*> print_data;
	map <string, LoanBookData*> :: iterator it = node_data->begin();
	parent = node;
	if (it->first[0] > end[0])
		return false;
	//If the data nodes only starts with the alpharbet that bigger than the alpharbet end, return the error code.
	while (it->first[0] <= end[0]) {
		if (parent == nullptr)
			break;
		node_data = parent->getDataMap();
		it = node_data->begin();
		if (it->first[0] <= end[0]) {
			print_data.insert(pair<string, LoanBookData*>(it->first, it->second));
			if (node_data->size() == 1)
				parent = parent->getNext();
			else {
				it++;
				if (it->first[0] <= end[0]) {
					print_data.insert(pair<string, LoanBookData*>(it->first, it->second));
					parent = parent->getNext();
				}
		/*	When the book title begins with two entered alphabets or with an alphabet between them,
			add to the print list.	*/
				else
					break;
			}
		}
		else
			break;
	/*	If the first alphabet of the book title is later in order
		than the alphabet passed as the last argument,list addition stops	*/
	}
	if (print_data.size() == 0)
		return false;
	ofstream flog;
	flog.open("log.txt", ios::app);
	flog << "============ SEARCH_BP ============" << endl;
    flog.close();
	for (it = print_data.begin(); it != print_data.end(); it++)
		it->second->printData();
	//	Prints book information in the list.
	flog.open("log.txt", ios::app);
    flog << "==================================" << endl << endl;
	flog.close();
	return true;
}

void BpTree::Delete(const string name) {
	BpTreeNode* node = searchDataNode(name);
	map<string, LoanBookData*>* data_node_map = node->getDataMap();
	map<string, LoanBookData*> ::iterator it_data = data_node_map->find(name);
	if (it_data == data_node_map->end())
		return;
	//	If there is no data corresponding to the entered name, the function ends.
	map<string, BpTreeNode*>* index_node_map;
	map<string, BpTreeNode*> :: iterator it_index;
	BpTreeNode* temp1;
	BpTreeNode* temp2;
	int flag = 0;
	if (data_node_map->size() == 2) {
		delete it_data->second;
		it_data = data_node_map->begin();
		temp1 = node->getParent();
		index_node_map = temp1->getIndexMap();
		it_index = index_node_map->find(it_data->first);
		/*	When the node that want to delete contains two book data, delete the corresponding book information.	*/
		if (it_index == index_node_map->end()) {
			temp1->insertIndexMap(it_data->first, node);
			it_index = index_node_map->find(name);
			it_index->second = nullptr;
			index_node_map->erase(it_index);
		}
		/*	 If the book is earlier in alphabetical order than other one, 
			change the key value of the parent index node to the remaining book title.	*/
		it_data = data_node_map->find(name);
	}
	data_node_map->erase(it_data);
	return;
}