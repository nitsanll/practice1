#include <windows.h>
#include <stdio.h>
#include "combobox.h"

HANDLE hStdin;
HANDLE hStdout;
DWORD fdwSaveOldMode; 
CONSOLE_SCREEN_BUFFER_INFO cbi;
DWORD wAttr2 = FOREGROUND_BLUE | FOREGROUND_INTENSITY;
DWORD wAttr3 = FOREGROUND_GREEN | FOREGROUND_INTENSITY;
COORD c[] = { { 7, 7 },{ 7,8 },{ 7,9 },{ 7,10 } };
string erase = "          ";

VOID ErrorExit(LPSTR);
VOID KeyEventProc(KEY_EVENT_RECORD, Combo);
VOID MouseEventProc(MOUSE_EVENT_RECORD, Combo);
VOID ResizeEventProc(WINDOW_BUFFER_SIZE_RECORD);
void printLines(Combo);
void eraseLines(string, int);
void changeTextColorDown(Combo, int);
void changeTextColorUp(Combo, int);

int main(VOID)
{
	DWORD cNumRead, fdwMode, i;
	INPUT_RECORD irInBuf[128];
	int counter = 0;

	// Get the standard input handle. 

	hStdin = GetStdHandle(STD_INPUT_HANDLE);
	if (hStdin == INVALID_HANDLE_VALUE)
		ErrorExit("GetStdHandle");

	hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	// Save the current input mode, to be restored on exit. 

	Combo combo;
	SetConsoleCursorPosition(hStdout, c[0]);

	CONSOLE_CURSOR_INFO cci = { 100, FALSE };
	SetConsoleCursorInfo(hStdout, &cci);

	SetConsoleTextAttribute(hStdout, wAttr2);

	cout << combo.getDeafult() + " " + 'V' << endl;
	
	SetConsoleCursorPosition(hStdout, c[0]);

	if (!GetConsoleMode(hStdin, &fdwSaveOldMode))
		ErrorExit("GetConsoleMode");

	// Enable the window and mouse input events. 

	fdwMode = ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT;
	if (!SetConsoleMode(hStdin, fdwMode))
		ErrorExit("SetConsoleMode");

	while (counter++ != EOF)
	{
		// Wait for the events. 

		if (!ReadConsoleInput(
			hStdin,      // input buffer handle 
			irInBuf,     // buffer to read into 
			128,         // size of read buffer 
			&cNumRead)) // number of records read 
			ErrorExit("ReadConsoleInput");

		// Dispatch the events to the appropriate handler. 

		for (i = 0; i < cNumRead; i++)
		{
			switch (irInBuf[i].EventType)
			{
			case KEY_EVENT: // keyboard input 
				KeyEventProc(irInBuf[i].Event.KeyEvent, combo);
				break;

			case MOUSE_EVENT: // mouse input 
				MouseEventProc(irInBuf[i].Event.MouseEvent, combo);
				break;

			case WINDOW_BUFFER_SIZE_EVENT: // scrn buf. resizing 
				ResizeEventProc(irInBuf[i].Event.WindowBufferSizeEvent);
				break;

			case FOCUS_EVENT:  // disregard focus events 

			case MENU_EVENT:   // disregard menu events 
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

VOID KeyEventProc(KEY_EVENT_RECORD ker, Combo combo)
{
	GetConsoleScreenBufferInfo(hStdout, &cbi);
	COORD coord = cbi.dwCursorPosition;
	int deafultLine = 7;

	const WORD up = VK_UP;
	const WORD down = VK_DOWN;
	const WORD enter = VK_RETURN;
	if (ker.bKeyDown) {
		if (ker.wVirtualKeyCode == up)
		{
			GetConsoleScreenBufferInfo(hStdout, &cbi);
			coord = cbi.dwCursorPosition;
			if (coord.Y == 10) {
				changeTextColorUp(combo, 2);
			}
			if (coord.Y == 9) {
				changeTextColorUp(combo, 1);
			}
		}

		if (ker.wVirtualKeyCode == down) 
		{ 
			if (coord.Y == deafultLine)
			{
				printLines(combo);
				changeTextColorDown(combo, 0);
			}
			
			else
			{
				GetConsoleScreenBufferInfo(hStdout, &cbi);
				coord = cbi.dwCursorPosition;
				printLines(combo);
				if (coord.Y == 8) changeTextColorDown(combo, 1);
				if (coord.Y == 9) changeTextColorDown(combo, 2);
				if (coord.Y == 10) changeTextColorDown(combo, 3);
			}
		}
		
		if (ker.wVirtualKeyCode == enter ) 
		{
			GetConsoleScreenBufferInfo(hStdout, &cbi);
			coord = cbi.dwCursorPosition;
			int i = coord.Y;
			combo.setDeafult(combo.getList().at(i-8));
			SetConsoleCursorPosition(hStdout, { 7,7 });
			SetConsoleTextAttribute(hStdout, wAttr2);
			cout << combo.getDeafult() + " " + 'V' << endl;
			eraseLines(erase, combo.getList().size());
			SetConsoleCursorPosition(hStdout, { 7,7 });
		}
	}
}

VOID MouseEventProc(MOUSE_EVENT_RECORD mer, Combo combo)
{
#ifndef MOUSE_HWHEELED
#define MOUSE_HWHEELED 0x0008
#endif
	if (mer.dwEventFlags == 0)
	{
		if (mer.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED)
		{
			int deafultLine = 7;
			vector<string> str = combo.getList();
			if (mer.dwMousePosition.Y == deafultLine && mer.dwMousePosition.X >= 7 && mer.dwMousePosition.X <= 12)
			{
				printLines(combo);
			}
			
			for (int i = 0, j=1; i < str.size(); i++, j++) {
				if (mer.dwMousePosition.Y == deafultLine + j && mer.dwMousePosition.X >= 7 && mer.dwMousePosition.X <= 12)
				{
					combo.setDeafult(str.at(i));
					SetConsoleCursorPosition(hStdout, {7,7});
					SetConsoleTextAttribute(hStdout, wAttr2);
					cout << combo.getDeafult() + " " + 'V' << endl;
					eraseLines(erase, str.size());
				}
			}
		}
	}
}

VOID ResizeEventProc(WINDOW_BUFFER_SIZE_RECORD wbsr)
{
	printf("Resize event\n");
	printf("Console screen buffer is %d columns by %d rows.\n", wbsr.dwSize.X, wbsr.dwSize.Y);
}

void printLines(Combo combo)
{
	COORD c[] = { { 7,8 },{ 7,9 },{ 7,10 } };
	CONSOLE_CURSOR_INFO cci = { 100, FALSE };
	SetConsoleCursorInfo(hStdout, &cci);
	SetConsoleTextAttribute(hStdout, wAttr2);
	for (int i = 0; i < combo.getList().size(); i++) {
		SetConsoleCursorPosition(hStdout, c[i]);
		cout << combo.getList().at(i);
	}
}

void eraseLines(string tmp, int size)
{
	COORD c1[] = { { 7,8 },{ 7,9 },{ 7,10 } };
	CONSOLE_CURSOR_INFO cci = { 100, FALSE };
	SetConsoleCursorInfo(hStdout, &cci);

	for (int i = 0; i < size; i++) {
		SetConsoleCursorPosition(hStdout, c1[i]);
		cout << tmp;
	}
}

void changeTextColorDown(Combo combo, int i)
{
	if (i == 3) { 
		SetConsoleCursorPosition(hStdout, c[i]);
		SetConsoleTextAttribute(hStdout, wAttr3);
		cout << combo.getList().at(i-1);
	}
	else {
		SetConsoleTextAttribute(hStdout, wAttr3);
		SetConsoleCursorPosition(hStdout, c[i + 1]);
		cout << combo.getList().at(i);
	}
}

void changeTextColorUp(Combo combo, int i)
{
	printLines(combo);
	SetConsoleTextAttribute(hStdout, wAttr3);
	SetConsoleCursorPosition(hStdout, c[i]);
	cout << combo.getList().at(i - 1);
}