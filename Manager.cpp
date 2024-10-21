#include "Manager.h"
#include <iostream>
#include <fstream>
#include <string>

void Manager::run(const char* command) 
{
	fin.open(command);
	if(!fin) {
		flog << "File Open Error" << endl;
		return;
	}
    Lheap = new LoanBookHeap*[8];
    for (int i = 0; i < 8; i++)
        Lheap[i] = new LoanBookHeap();
	string cmd;
	bool operation = false;
    //flog.open("log.txt", ios::app);
    flog.open("log.txt");
	while (!fin.eof()) {
		getline(fin, cmd);
        //Get a single line of commands.
        if (cmd.compare("LOAD") == 0) {
			operation = Load();
            if (operation == true)
				printSuccessCode("LOAD");
			else
                printErrorCode(100);
        }
        /*  If the command is "LOAD", executes the function Load.
            If the error occurs in the function, write the error message to the log file.  */
        else if (cmd.substr(0, 3).compare("ADD") == 0) {
            cmd = cmd.substr(4);   //Extract member information from the command.
			operation = Add(cmd);
            if (!operation)
                printErrorCode(200);
        }
        /*  If the command starts with "ADD", executes the function Add.
            If the error occurs in the function, write the error message to the log file.  */
        else if (cmd.substr(0, 6).compare("SEARCH") == 0) {
            cmd = cmd.substr(10);   //Extract member name from the command.
			if (cmd.length() == 3 && cmd[1] == ' ')
				operation = Search_bp_range(cmd.substr(0, 1), cmd.substr(2, 1));
			else 
				operation = Search_bp_book(cmd);
			if (!operation)
				printErrorCode(300);
        }
        /*  If the command starts with "SEARCH", find the searching option.
			If the option is alphabet, executes the function Search_bp_range.
			If not, executes the function Search_bp_book.
            If the error occurs in the function, write the error message to the log file.  */
		else if (cmd == "PRINT_BP") {
            operation = Print_bp();
			if (!operation)
				printErrorCode(400);
        }
        /*  If the command starts with "PRINT_BP", executes the Print_bp function.
            If the error occurs in the function, write the error message to the log file.  */
        else if (cmd.substr(0, 8) == "PRINT_ST") {
            cmd = cmd.substr(9);   //Extract the print condition from the command.
            operation = Print_st(stoi(cmd));
			if (!operation)
				printErrorCode(500);
        }
        /*  If the command starts with "PRINT_ST", executes the Print_st function.
            If the error occurs in the function, write the error message to the log file.  */
        else if (cmd == "DELETE") {
			operation = Delete();
            if (operation)
                printSuccessCode("DELETE");
            else
                printErrorCode(600);
        }
        /*  If the command starts with "DELETE", executes the function Delete.
            If the error occurs in the function, write the error message to the log file.  */
        else if (cmd.compare("EXIT") == 0) {
            break;
        }
        /*  If the command is "EXIT", Executes the function Exit.
            If the error occurs in the function, write the error message to the log file.
            If not, outputs the success code.
            After these actions, close the log file and exit from the loop.  */
        else 
            printErrorCode(700);
        //If the wrong command is entered, write the error message.
    }
    for (int i = 0; i < 8; i++) {
        while (Lheap[i]->getRoot()->getLeftChild() != nullptr && Lheap[i]->getRoot()->getLeftChild() != nullptr)
            Lheap[i]->Delete(Lheap[i]->getRoot());
    }
    printSuccessCode("EXIT");
    flog.close();
	return;
}

bool Manager::Load() {
	ifstream loan_book;
	loan_book.open("loan_book.txt");
	if (!loan_book)
		return false;
	//If the "loan_book.txt" file does not exist, return false and terminate the function.
	string book_info, name, author;
    int code, year, loan_count, temp, count;
	while (!loan_book.eof()) {
        code = -1;
        year = -1;
        loan_count = -1;
        temp = 0;
        name = ".";
        author = ".";
        //  Initialize variables to store book information.
		getline(loan_book, book_info);
        if (book_info.length() < 14)
            continue;
        /*  If the length of the string received from "loan_book.txt" is less than 14,
            inputting that information is skipped.
            (The minimum number of characters to enter book information is 14 characters.)    */
        count = book_info.find("\t", temp);
        name = book_info.substr(0, count);
        //Extract the book name.
        if (name.length() > 50) 
            continue;
        
        try {
            author = book_info.substr(count+1, 3);
            code = stoi(author);
            //Extract the book classification code.
            temp = count + 5;
            count = book_info.find("\t", temp);
            year = stoi(book_info.substr(count+1, 4));
            //Extract the publication year.
        }
        catch (...) {
            continue;
        }
        //  If the stoi error is occured, skip the inputting book information.
        loan_count = book_info.back() - '0';
        //Extract the loan count of the book.
        author = book_info.substr(temp, count - temp);
        //Extract the author of the book.
        if (code % 100 != 0)
            continue;
        
        // If wrong classification code is entered, skip the inputting information.
        if (code >= 0 && code <= 200) {
            if (loan_count >= 3) {
                if (loan_count == 3)
                    count = -2;
                else
                    continue;
            }
        }
        else if (code >= 300 && code <= 400) {
            if (loan_count >= 4) {
                if (loan_count == 4)
                    count = -2;
                else
                    continue;
            }
        }
        else if (code >= 500 && code <= 700) {
            if (loan_count >= 2) {
                if (loan_count == 2)
                    count = -2;
                else
                    continue;
            }
        }
        /*  Entering the information is omitted if more “loan count” values ​​are entered
            than the number of books available for loan corresponding to the book code number.  */
        LoanBookData* book = new LoanBookData();
        book->setBookData(name, code, author, year, loan_count);
        if (count != -2)
            count = bptree->Insert(book);
        /*  if less loan count is entered than the available amount,
            insert the book data into the b+ tree data structure. */
        if (count == -2) {
            LoanBookHeapNode* temp = Lheap[code/100]->getRoot();
            Lheap[code/100]->Insert(book);
        /*  If the node's loan count is same with the number of available loan volumes,
            insert in loan heap corresponding to its classification code.  */
            if (temp != Lheap[code/100]->getRoot())
                stree->Insert(book, Lheap[code/100]);
            /*  if the root of heap is changed, restruct the selection tree.    */
        }
	}
	return true;
}

bool Manager::Add(string book_info) {
    string name, author;
    int count = 0, temp = 0, year = 0, code = 0;
    if (book_info.length() < 13) {
        if (book_info.length() > 1)
        return false;
    }
    /*  If the length of the string received from "loan_book.txt" is less than 13,
        inputting that information is skipped.
        (The minimum number of characters to enter book information is 13 characters.)    */
    count = book_info.find("\t", temp);
    name = book_info.substr(0, count);
    //Extract the book name.
    if (name.length() > 50){
        return false;
    }
    try {
        author = book_info.substr(count+1, 3);
        code = stoi(author);
        //Extract the book classification code.
        temp = count + 5;
        count = book_info.find("\t", temp);
        year = stoi(book_info.substr(count+1, 4));
        //  Extract the publication year.
    }
    catch (...) {
        return false;
    }
    //  If the stoi error is occured, skip the inputting book information.
    author = book_info.substr(temp, count - temp);
    LoanBookData* book = new LoanBookData();
    book->setBookData(name, code, author, year, 0);
    count = bptree->Insert(book);
    if (count == -2) {
        if (code >= 0 && code <= 200)
            count = 3;
        else if (code >= 300 && code <= 400)
            count = 4;
        else
            count = 2;
        for (int i = 0; i < count; i++) {
            book->updateCount();
        }
        count = -2;
        LoanBookHeapNode* temp = Lheap[code/100]->getRoot();
        Lheap[code/100]->Insert(book);
    /*  If the node's loan count is same with the number of available loan volumes,
        insert in loan heap corresponding to its classification code.  */
        if (temp != Lheap[code/100]->getRoot())
            stree->Insert(book, Lheap[code/100]);
        
    }
    else {
        for (int i = 0; i < count; i++) {
            book->updateCount();
        }
    }
    flog << "============== ADD ==============" << endl;
    flog.close();
    book->printData();
    flog.open("log.txt", ios::app);
    flog << "=================================" << endl << endl;
    //Print the book data.
    if (count != -2)
        delete book;
	return true;
}

bool Manager::Search_bp_book(string book) {
    flog.close();
    bool result = bptree->searchBook(book);
    flog.open("log.txt", ios::app);
    //Execute the search book function that included in the BpTree class.
	return result;
}

bool Manager::Search_bp_range(string s, string e) {
    flog.close();
    bool result = bptree->searchRange(s, e);
    flog.open("log.txt", ios::app);
    //Execute the search range function that included in the BpTree class.
    return result;
}

bool Manager::Print_bp() {
    BpTreeNode* node_it = bptree->getRoot();
    if (node_it == nullptr) return false;
    //If there's no book information in b+ tree, then return the error code.
    map<string, LoanBookData*>* data;
    map<string, LoanBookData*> :: iterator data_it;
    while (node_it->getDataMap() == nullptr) {
        if (node_it->getMostLeftChild() != nullptr)
            node_it = node_it->getMostLeftChild();
        else
            node_it = node_it->getIndexMap()->begin()->second;
    }
    //Find the data node with the smallest key value.
    flog << "============ PRINT_BP ============" << endl;
    flog.close();
	while (node_it != nullptr) {
        data = node_it->getDataMap();
        data_it = data->begin();
        data_it->second->printData();
        //Print the first book data of data node.         
        if (data->size() > 1) {
            data_it++;
            data_it->second->printData();
        }
        //If there's two book datas, print another one.
        node_it = node_it->getNext();
        //Move to next data node.
    }
    //Executes when the node reaches the end of data node.
    flog.open("log.txt", ios::app);
    flog << "==================================" << endl << endl;
	return true;
}

bool Manager::Print_st(int type) {
    flog.close();
    bool result = stree->printBookData(type);
    flog.open("log.txt", ios::app);
    //Execute the search range function that included in the BpTree class.
    return result;
}
bool Manager::Delete() {
    bool result = stree->Delete();
    //Execute the search range function that included in the BpTree class.
	return result;
}

void Manager::printSuccessCode(string cmd) {
/*//////////////////////////////////////////////////////////////////////////
                function name : printSuccessCode
        Input : string - command that executed
        Output : None
        Purpose : Print the success message.
//////////////////////////////////////////////////////////////////////////*/
    flog << "============" << cmd << "============" << endl;
    flog << "Success" << endl;
    flog << "==============================" << endl << endl;
}

void Manager::printErrorCode(int n) {
/*//////////////////////////////////////////////////////////////////////////
                function name : printErrorCode
        Input : int - command that executed
        Output : None
        Purpose : Print the success message.
//////////////////////////////////////////////////////////////////////////*/
    flog << "============ ERROR ============" << endl;
    flog << n << endl;
    flog << "===============================" << endl << endl;
}
