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
	/*VOID ErrorExit(LPSTR, HANDLE, DWORD);
	VOID KeyEventProc(KEY_EVENT_RECORD, HANDLE, int, Combo*);
	VOID MouseEventProc(MOUSE_EVENT_RECORD, HANDLE, int, Combo*);
	VOID ResizeEventProc(WINDOW_BUFFER_SIZE_RECORD);
	void SetChoice(string);
	string GetChoice();
	*/
	vector<string> getList();
	string getDeafult();
	void setDeafult(string str);
	};

#endif