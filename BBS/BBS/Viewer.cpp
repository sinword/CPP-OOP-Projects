#include "Viewer.h"



Viewer::Viewer() {
	//初始化視窗大小
	
	TCHAR title[MAX_PATH] = TEXT("BBS");
	SetConsoleTitle(title);
	Sleep(40);
	HWND hwnd;
	hwnd = FindWindow(NULL, title);
	MoveWindow(hwnd, 100, 100, 1936, 1080, true);
	
	
		//鎖定視窗大小
	SetWindowLongPtrA(
		GetConsoleWindow(),
		GWL_STYLE,
		GetWindowLongPtrA(GetConsoleWindow(), GWL_STYLE)
		& ~WS_SIZEBOX & ~WS_MAXIMIZEBOX & ~WS_MINIMIZEBOX
	);
	fontSize(40, 40);
	//1920/32=60 中文字60英文120 寬  高 中英皆32
	
	for (int i = 0; i < height; ++i) {//初始化顏色
		for (int j = 0; j < width; ++j) {
			color[i][j] = 7;
		}
	}



}
string Viewer::getInput(int y, int x, int length) {
	string tmp = "";
	char chr;
	while (true) {
		gotoyx(y, x+tmp.length());
		chr = _getch();
		string tt = "";
		tt += chr;
		if (chr&0x80) {
			if (tmp.length() + 1 < length) {
				tt += _getch();
				tmp += tt;
				cout << tt;
			}
		}
		else if (isalnum(chr)) {
			if (tmp.length() < length) {
				tmp += chr;
				cout << chr;
			}
			else {
				continue;
			}
		}
		else if (chr == 8) {//backspace
			if (tmp.length() > 1) {
				if (tmp[tmp.length() - 2] & 0x80&&tmp[tmp.length()-1]&0x80) {
					gotoyx(y, x + tmp.length() - 2);
					cout << "  ";
					tmp = string(tmp.begin(), tmp.end() - 2);
				}
				else {
					gotoyx(y, x + tmp.length() - 1);
					cout << " ";
					tmp = string(tmp.begin(), tmp.end() - 1);
				}
			}
			else if (tmp.length() > 0) {
				gotoyx(y, x + tmp.length() - 1);
				cout << " ";
				tmp = string(tmp.begin(), tmp.end() - 1);
			}
			else {
				continue;
			}
		}
		else if (chr == 13) {//enter
			if (tmp.length() != 0) {
				return tmp;
			}
			else {
				continue;
			}
		}
		else if (chr == 27) {//esc
			return string();
		}
		
	}
	
	
}
void Viewer::dHorse(int y, int x,int color) {
	int horse[][5] = { {0,0,1,0,1},
					   {0,0,0,1,1},{
						1,1,1,1,1},{
						1,0,1,0,0},{
						1,0,1,0,0} };
	for (int i = 0; i < 5; ++i) {
		for (int j = 0; j < 5; ++j) {
			gotoyx(y + i, x + j);
			if (horse[i][j]) {
				SetColor(color);
			}
			else {
				SetColor(7);
			}
			cout << " ";
		}
	}
	SetColor(7);
}

void Viewer::clearColor() {
	for (int i = 0; i < height; ++i) {
		for (int j = 0; j < width; ++j) {
			color[i][j] = 7;
		}
	}
}
void SetColor(int color = 7)
{
	HANDLE hConsole;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, color);
}
void fontSize(int w,int h) {
	CONSOLE_FONT_INFOEX font;
	font.cbSize = sizeof(CONSOLE_FONT_INFOEX);
	GetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), false, &font);
	font.dwFontSize.X = w;
	font.dwFontSize.Y = h;
	SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), false, &font);
}
void Viewer::clear() {
	for (int i = 0; i < height; ++i) {
		for (int j = 0; j < width; ++j) {
			gotoyx(i, j);
			cout << " ";
			screen[i][j] = "";
		}
	}
	clearColor();
}
void Viewer::cOneline(int line,int color) {
	SetColor(color);
	for (int i = 0; i < width; ++i) {
		gotoyx(line, i);
		cout << " ";
		screen[line][i] = " ";
	}
}
void Viewer::setWord(int y, int x, string word) {
	
	for (int i = 0; i < word.length();) {
		if (x > width) {
			y++;
			x -= width;
		}
		if (word[i] == '\n') {
			x = 0;
			y++;
			i++;
			continue;
		}
		if (word[i] & 0x80&&i+1<word.length()) {
			string tmp(2, '0');
			
			tmp[0] = word[i];
			tmp[1] = word[i+1];
			screen[y][x] = tmp;
			x += 2;
			i += 2;
		}
		else {
			string tmp(1, '0');
			tmp[0] = word[i];
			screen[y][x] = tmp;
			x++;
			i++;
		}
	}
}


void Viewer::draw() {
	
	for (int i = 0; i < height; ++i) {
		for (int j = 0; j < width; ++j) {
			gotoyx(i, j);
			SetColor(color[i][j]);
			cout << screen[i][j];
		}
	}
	SetColor(7);
}
void Viewer::setNdraw(int y, int x, string word) {
	int originX = x;
	for (int i = 0; i < word.length();) {
		if (x > width) {
			y++;
			x -= width;
		}
		if (word[i] == '\n') {
			x = originX;
			y++;
			i++;
			continue;
		}
		if (word[i] & 0x80 && i + 1 < word.length()) {
			string tmp(2, '0');

			tmp[0] = word[i];
			tmp[1] = word[i + 1];
			screen[y][x] = tmp;
			gotoyx(y, x);
			SetColor(color[y][x]);
			cout << tmp;
			x += 2;
			i += 2;
		}
		else {
			string tmp(1, '0');
			tmp[0] = word[i];
			screen[y][x] = tmp;
			gotoyx(y, x);
			SetColor(color[y][x]);
			cout << tmp;
			x++;
			i++;
		}
	}
}
void gotoyx(int ypos, int xpos)
{
	COORD scrn;
	HANDLE hOuput = GetStdHandle(STD_OUTPUT_HANDLE);
	scrn.X = xpos; scrn.Y = ypos;
	SetConsoleCursorPosition(hOuput, scrn);
}

