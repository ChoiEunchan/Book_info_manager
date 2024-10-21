#pragma once
#include<iostream>
#include<cstring>
#include <fstream>

using namespace std;

class LoanBookData {
private:
    string name;
    int code;
    string author;
    int year;
    int loan_count;

public:
    LoanBookData() { this->loan_count = 0; }
    ~LoanBookData() {};

    void setBookData(string name, int code, string author, int year, int laon_count) {
        this->name = name;
        this->code = code;
        this->author = author;
        this->year = year;
        this->loan_count = laon_count;
    }

    void setName(string name) { this->name = name; }
    void setCode(int code) { this->code = code; }
    void setAuthor(string author) { this->author = author; }
    void setYear(int year) { this->year = year; }
    void updateCount() { this->loan_count += 1; }

    string getName() { return name; }
    int getCode() { return code; }
    string getAuthor() { return author; }
    int getYear() { return year; }
    int getLoanCount() { return loan_count; }

    void printData() {
        ofstream fout;
        fout.open("log.txt", ios::app);
        fout << name << "/";
        if (code == 0)
            fout << "00";
		fout << code << "/";
		fout << author << "/";
		fout << year << "/";
		fout << loan_count << endl;
        fout.close();
        return;
    }
};