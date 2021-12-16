#ifndef _API_H_
#define _API_H_

#include <Windows.h>
#include <cstring>
#include <vector>
#include <iostream>

const int MAP_HEIGHT = 36;
const int MAP_WIDTH = 100;

const int CONSOLE_WIDTH = 150;
const int CONSOLE_HEIGHT = 36;

using namespace std;

void fixConsole() {
    HWND consoleWindow = GetConsoleWindow();
    LONG style = GetWindowLong(consoleWindow, GWL_STYLE);
    style = style & ~(WS_MAXIMIZEBOX) & ~(WS_THICKFRAME);
    SetWindowLong(consoleWindow, GWL_STYLE, style);
}

void resizeConsole(int width, int height) {
    HWND console = GetConsoleWindow();
    RECT r;
    GetWindowRect(console, &r);
    MoveWindow(console, r.left, r.top, width, height, TRUE);
}

void textColor(int x) {
    HANDLE color;
    color = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(color, x);
}

void gotoxy(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void hideCursor() {
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 1;
    info.bVisible = FALSE;
    SetConsoleCursorInfo(consoleHandle, &info);
}


wstring ExePath() {
	TCHAR buffer[MAX_PATH] = { 0 };
	GetModuleFileName(NULL, buffer, MAX_PATH);
	std::wstring::size_type pos = std::wstring(buffer).find_last_of(L"\\/");
	return std::wstring(buffer).substr(0, pos);
}

void listTXT() {
	vector<wstring> names;
	wstring temp = ExePath();
	while (temp.back() != '\\') {
		temp.pop_back();
	}
	temp += L"\\DOANCUOIKY\\*.txt";
	WIN32_FIND_DATA fd;
	HANDLE hFind = ::FindFirstFile(temp.c_str(), &fd);
	if (hFind != INVALID_HANDLE_VALUE) {
		do {
			if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
				names.emplace_back(fd.cFileName);
			}
		} while (::FindNextFile(hFind, &fd));
		::FindClose(hFind);
	}
	for (int i = 0; i < 10; i++) {
		gotoxy(CONSOLE_WIDTH / 2 - 14, MAP_HEIGHT / 2 + 1 + i);
		cout << i + 1 << ".                           ";
	}
	int n = names.size();
	for (auto i = 0; i < names.size(); i++) {
		wstring fileName = names[i];
		gotoxy(CONSOLE_WIDTH / 2 - 10, MAP_HEIGHT / 2 + 1 + i);
		do {
			fileName.pop_back();
		} while (fileName.back() != '.');
		fileName.pop_back();
		wcout << fileName << endl;
	}
	if (n < 10) {
		for (int i = 9; i >= n; --i) {
			textColor(8);
			gotoxy(CONSOLE_WIDTH / 2 - 10, MAP_HEIGHT / 2 + 1 + i);
			cout << "EMPTY";
		}
		textColor(14);
	}
}
#endif
