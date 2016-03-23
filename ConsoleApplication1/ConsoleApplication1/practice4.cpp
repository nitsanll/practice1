#include <windows.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <string>

using namespace std;

HANDLE hStdin;
HANDLE hStdout;
DWORD fdwSaveOldMode;
CONSOLE_SCREEN_BUFFER_INFO cbi;

VOID ErrorExit(LPSTR);
VOID KeyEventProc(KEY_EVENT_RECORD);
VOID MouseEventProc(MOUSE_EVENT_RECORD);
VOID ResizeEventProc(WINDOW_BUFFER_SIZE_RECORD);

class Radio {
	public: 
		Radio();	
		void draw();
		void selected(int i);
	private: 
		vector<string> str_arr;
};

Radio::Radio() {
	int i = 0;
	while (i<3) {
		string str = "[]choose me";
		i++;
		str_arr.push_back(str);
	}
	draw();
}

void Radio::draw() {
	/*COORD c = { 20, 10 };
	hStdin = GetStdHandle(STD_INPUT_HANDLE);
	if (hStdin == INVALID_HANDLE_VALUE)
		ErrorExit("GetStdHandle");
	SetConsoleCursorPosition(hStdin, c);

	CONSOLE_CURSOR_INFO cci = { 100, FALSE };
	SetConsoleCursorInfo(hStdin, &cci);
	*/

	COORD c[] = { { 7, 7 }, { 7,8 }, { 7,9 } };
	
	SetConsoleCursorPosition(hStdout, c[0]);
	//printf("fuckin a\n");

	CONSOLE_CURSOR_INFO cci = { 100, FALSE };
	SetConsoleCursorInfo(hStdin, &cci);

	for (int i = 0; i < str_arr.size(); i++) {
		cout << str_arr.at(i);
		SetConsoleCursorPosition(hStdout, c[i+1]);
	}
}

void Radio::selected(int i) {
	COORD c = cbi.dwCursorPosition;

	DWORD wAttr2 = cbi.wAttributes | BACKGROUND_BLUE | BACKGROUND_GREEN;
	SetConsoleTextAttribute(hStdin, wAttr2);
}

int main(VOID)
{
	//radio.selected(0);

	DWORD cNumRead, fdwMode, i;
	INPUT_RECORD irInBuf[20];
	int counter = 0;

	hStdin = GetStdHandle(STD_INPUT_HANDLE);
	if (hStdin == INVALID_HANDLE_VALUE)
		ErrorExit("GetStdHandle");

	hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	// Save the current input mode, to be restored on exit. 
	
	Radio radio;
	
	if (!GetConsoleMode(hStdin, &fdwSaveOldMode))
		ErrorExit("GetConsoleMode");
	// Enable the window and mouse input events. 

	fdwMode = ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT;
	if (!SetConsoleMode(hStdin, fdwMode))
		ErrorExit("SetConsoleMode");

	// Loop to read and handle the next 100 input events. 

	

	while (counter++ <= 20)
	{
		// Wait for the events. 

		if (!ReadConsoleInput(
			hStdin,      // input buffer handle 
			irInBuf,     // buffer to read into 
			20,         // size of read buffer 
			&cNumRead)) // number of records read 
			ErrorExit("ReadConsoleInput");

		// Dispatch the events to the appropriate handler. 

		for (i = 0; i < cNumRead; i++)
		{
			switch (irInBuf[i].EventType)
			{
			case KEY_EVENT: // keyboard input 
				KeyEventProc(irInBuf[i].Event.KeyEvent);
				break;

			case MOUSE_EVENT: // mouse input 
				MouseEventProc(irInBuf[i].Event.MouseEvent);
				break;

			default:
				ErrorExit("Unknown event type");
				break;
			}
		}
	}

	// Restore input mode on exit.

	SetConsoleMode(hStdin, fdwSaveOldMode); 

	return 0;
}

VOID ErrorExit(LPSTR lpszMessage)
{
	fprintf(stderr, "%s\n", lpszMessage);

	// Restore input mode on exit.

	SetConsoleMode(hStdin, fdwSaveOldMode);

	ExitProcess(0);
}

VOID KeyEventProc(KEY_EVENT_RECORD ker)
{
	printf("hi");
	const WORD up = VK_UP; 
	const WORD down = VK_DOWN;
	const WORD enter = VK_RETURN;
	if (ker.bKeyDown) {
		if (ker.wVirtualKeyCode == up) {}
		if (ker.wVirtualKeyCode == down) { cout << "down"<< endl; }
		if (ker.wVirtualKeyCode == enter) {}
	}
	
}

VOID MouseEventProc(MOUSE_EVENT_RECORD mer)
{
#ifndef MOUSE_HWHEELED
#define MOUSE_HWHEELED 0x0008
#endif
	printf("Mouse event: ");

	switch (mer.dwEventFlags)
	{
	case 0:

		if (mer.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED)
		{
			printf("left button press \n");
		}
		else if (mer.dwButtonState == RIGHTMOST_BUTTON_PRESSED)
		{
			printf("right button press \n");
		}
		else
		{
			printf("button press\n");
		}
		break;
	case DOUBLE_CLICK:
		printf("double click\n");
		break;
	case MOUSE_HWHEELED:
		printf("horizontal mouse wheel\n");
		break;
	case MOUSE_MOVED:
		printf("mouse moved\n");
		break;
	case MOUSE_WHEELED:
		printf("vertical mouse wheel\n");
		break;
	default:
		printf("unknown\n");
		break;
	}
}

VOID ResizeEventProc(WINDOW_BUFFER_SIZE_RECORD wbsr)
{
	printf("Resize event\n");
	printf("Console screen buffer is %d columns by %d rows.\n", wbsr.dwSize.X, wbsr.dwSize.Y);
}
