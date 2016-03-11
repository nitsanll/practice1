#include <windows.h>
#include <stdio.h>
#include <iostream>
#include <vector>

using namespace std;

class Radio {
	public: 
		Radio();
		void print_vec();
	private: 
		vector<string> str_arr;
};

Radio::Radio() {
	int i = 0;
	while (i<3) {
		string str = "choose me: no. " + i;
		i++;
		str_arr.push_back(str);
	}
}

void Radio::print_vec() {
	int i = 0;
	while (i<str_arr.size) {
		
		i++;
	}
}

int main(VOID)
{
	COORD c = { 20, 7 };
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(h, c);
	
	Radio radio;
	while (i<3) {
		cout << "choose me: no. " << i;
		printf("\n");
		i++;
	}

	CONSOLE_CURSOR_INFO cci = { 100, FALSE };
	SetConsoleCursorInfo(h, &cci);

	return 0;
}
