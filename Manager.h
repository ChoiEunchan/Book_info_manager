#pragma once
#include "SelectionTree.h"
#include "BpTree.h"

class Manager {
private:
	BpTree* bptree;
	SelectionTree* stree;
	LoanBookHeap** Lheap;
public:
	Manager(int bpOrder) {
		bptree = new BpTree(bpOrder);
		stree = new SelectionTree();
	}
	~Manager() {
		delete bptree;
		delete stree;
		for (int i = 0; i < 8; i++)
			delete Lheap[i];
		delete[] Lheap;
	}

	ifstream fin;
	ofstream flog;
	
	void run(const char* command);
	bool Load();
	bool Add(string information);

	bool Search_bp_book(string book);
	bool Search_bp_range(string start, string end);

	bool Print_bp();
	bool Print_st(int type);

	bool Delete();

	void printErrorCode(int n);
	void printSuccessCode(string cmd);
};

