#ifndef COMBOBOX_H_
#define COMBOBOX_H_
#include <windows.h>
#include <string>
#include <iostream>
#include <vector>


using namespace std;

class Combo {
private:
	vector<string> list;
	string choice;
	string deafult;

public:
	Combo();
	~Combo();
	vector<string> getList();
	string getDeafult();
	void setDeafult(string str);
	};

#endif