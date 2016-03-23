#include <windows.h>
#include <stdio.h>
#include "combobox.h"

HANDLE hStdin;
HANDLE hStdout;
DWORD fdwSaveOldMode; 
CONSOLE_SCREEN_BUFFER_INFO cbi;
COORD c[] = { { 7, 7 },{ 7,8 },{ 7,9 },{ 7,10 } };

VOID ErrorExit(LPSTR);
VOID KeyEventProc(KEY_EVENT_RECORD, Combo);
VOID MouseEventProc(MOUSE_EVENT_RECORD, Combo);
VOID ResizeEventProc(WINDOW_BUFFER_SIZE_RECORD);
void printLines(Combo);
void eraseLines(string, int);

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
	//COORD c[] = { { 7, 7 },{ 7,8 },{ 7,9 }, {7,10} };

	SetConsoleCursorPosition(hStdout, c[0]);

	CONSOLE_CURSOR_INFO cci = { 100, FALSE };
	SetConsoleCursorInfo(hStdout, &cci);

	cout << combo.getDeafult() + " " + '\xC2' << endl;

	/*for (int i = 0; i < combo.getList().size(); i++) {
	cout << combo.getList().at(i);
	SetConsoleCursorPosition(hStdout, c[i + 1]);
	}*/

	if (!GetConsoleMode(hStdin, &fdwSaveOldMode))
		ErrorExit("GetConsoleMode");

	// Enable the window and mouse input events. 

	fdwMode = ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT;
	if (!SetConsoleMode(hStdin, fdwMode))
		ErrorExit("SetConsoleMode");

	// Loop to read and handle the next 100 input events. 

	
	
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
		if (ker.wVirtualKeyCode == up) {}
		if (ker.wVirtualKeyCode == down) 
		{ 
			if (coord.Y == deafultLine+1)
			{
				printLines(combo);
				DWORD wAttr2 = cbi.wAttributes | BACKGROUND_BLUE | BACKGROUND_GREEN;
				SetConsoleTextAttribute(hStdout, wAttr2);
				SetConsoleCursorPosition(hStdout, c[1]);
				cout << combo.getList().at(0);
				SetConsoleCursorPosition(hStdout, c[2]);
			}
			
			else
			{
					GetConsoleScreenBufferInfo(hStdout, &cbi);
					coord = cbi.dwCursorPosition;
					cout << coord.Y;
					printLines(combo);
					/*DWORD wAttr2 = cbi.wAttributes | BACKGROUND_BLUE | BACKGROUND_GREEN;
					SetConsoleTextAttribute(hStdout, wAttr2);
					SetConsoleCursorPosition(hStdout, coord);
					cout << combo.getList().at(i + 1);
				*/
			}
		}
		if (ker.wVirtualKeyCode == enter) {}
	}
}

VOID MouseEventProc(MOUSE_EVENT_RECORD mer, Combo combo)
{
#ifndef MOUSE_HWHEELED
#define MOUSE_HWHEELED 0x0008
#endif
	//printf("Mouse event: ");

	switch (mer.dwEventFlags)
	{
	case 0:

		if (mer.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED)
		{
			int deafultLine = 7;
			vector<string> str = combo.getList();
			string erase = "          ";
			if (mer.dwMousePosition.Y == deafultLine && mer.dwMousePosition.X >= 7 && mer.dwMousePosition.X <= 12)
			{
				printLines(combo);
			}
			
			for (int i = 0, j=1; i < str.size(); i++, j++) {
				if (mer.dwMousePosition.Y == deafultLine + j && mer.dwMousePosition.X >= 7 && mer.dwMousePosition.X <= 12)
				{
					combo.setDeafult(str.at(i));
					SetConsoleCursorPosition(hStdout, {7,7});
					cout << combo.getDeafult() + " " + '\xC2' << endl;
					eraseLines(erase, str.size());

				}
			}
		}
		/*else if (mer.dwButtonState == RIGHTMOST_BUTTON_PRESSED)
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
		*/
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
