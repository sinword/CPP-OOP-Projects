#include "Viewer.h"
void voidClick() {
	while (true) {
		if (GetAsyncKeyState(VK_LBUTTON)) {
			break;
		}
	}
}
void ShowConsoleCursor(bool showFlag)
{
	HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);

	CONSOLE_CURSOR_INFO     cursorInfo;

	GetConsoleCursorInfo(out, &cursorInfo);
	cursorInfo.bVisible = showFlag; // set the cursor visibility
	SetConsoleCursorInfo(out, &cursorInfo);
}
void gotoyx(int ypos, int xpos)
{
	COORD scrn;
	HANDLE hOuput = GetStdHandle(STD_OUTPUT_HANDLE);
	scrn.X = xpos; scrn.Y = ypos;
	SetConsoleCursorPosition(hOuput, scrn);
}
void SetColor(int color = 7)
{
	HANDLE hConsole;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, color);
}
void clearS() {
	COORD topLeft = { 0, 0 };
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO screen;
	DWORD written;

	GetConsoleScreenBufferInfo(console, &screen);
	FillConsoleOutputCharacterA(
		console, ' ', screen.dwSize.X * screen.dwSize.Y, topLeft, &written
	);
	FillConsoleOutputAttribute(
		console, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE,
		screen.dwSize.X * screen.dwSize.Y, topLeft, &written
	);
	SetConsoleCursorPosition(console, topLeft);
}
void Viewer::drawTime(int player, int time) {
	SetColor(7);
	int hours = time / 3600;
	time %= 3600;
	int minutes = time / 60;
	time %= 60;
	int seconds = time;
	
	if (player == 0) {
		gotoyx(41, 130);
		cout << hours << ":" << minutes << ":" << seconds << "  ";
	}
	else {
		gotoyx(7, 130);
		cout << hours << ":" << minutes << ":" << seconds << "  ";
	}
	SetColor(0);

}
void Viewer::miniSqure(string word, int y, int x,bool choose) {
	if (choose) {
		SetColor(238);
	}
	else {
		SetColor(127);
	}
	gotoyx(y, x);
	cout << "       ";
	gotoyx(y + 1, x);
	cout << " ";
	gotoyx(y + 1, x+6);
	cout << " ";
	gotoyx(y + 2, x);
	cout << " ";
	gotoyx(y + 2, x + 6);
	cout << " ";
	gotoyx(y + 3, x);
	cout << " ";
	gotoyx(y + 3, x + 6);
	cout << " ";
	gotoyx(y+4, x);
	cout << "       ";
	SetColor(15);
	gotoyx(y + 2, x + 3);
	cout << word;
	SetColor();
}

void Viewer::Setting(int AI,int ForL) {
	clearS();
	gotoyx(10, 10);
	cout << "AI diff (0 for two players)  ";
	miniSqure("0", 8, 40, AI==0);
	miniSqure("1", 8, 47, AI==1);

	gotoyx(20, 10);
	cout << "White or Black  ";
	if (AI == 0) {
		miniSqure("W", 18, 27, false);
		miniSqure("B", 18, 34, false);
	}
	else {
		miniSqure("W", 18, 27, ForL == 1);
		miniSqure("B", 18, 34, ForL == 2);
	}

	gotoyx(30, 10);
	SetColor(253);
	cout << "        ";
	gotoyx(31, 10);
	cout << " return ";
	gotoyx(32, 10);
	cout << "        ";
	SetColor();


}

Viewer::Viewer() {
	drawing = false;
	table.resize(8);			//初始化輸出用棋盤
	for (int i = 0; i < 8; ++i)
		table[i].resize(8);
	owner.resize(8);			//初始化輸出用棋盤
	for (int i = 0; i < 8; ++i)
		owner[i].resize(8);
	screen.resize(56);
	for (int i = 0; i < 56; ++i)
		screen[i].resize(104);
	
	//初始化視窗大小
	
	TCHAR title[MAX_PATH] = TEXT("Chess Game");
	SetConsoleTitle(title);
	Sleep(40);
	HWND hwnd;
	hwnd = FindWindow(NULL, title);
	MoveWindow(hwnd, 100, 30, 1400, 1000, true);
	//鎖定視窗大小
	SetWindowLongPtrA(
		GetConsoleWindow(),
		GWL_STYLE,
		GetWindowLongPtrA(GetConsoleWindow(), GWL_STYLE)
		& ~WS_SIZEBOX & ~WS_MAXIMIZEBOX & ~WS_MINIMIZEBOX
	);


	CONSOLE_FONT_INFOEX font;
	font.cbSize = sizeof(CONSOLE_FONT_INFOEX);
	GetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), false, &font);
	fontW = font.dwFontSize.X;	//8
	fontH = font.dwFontSize.Y;	//16
}
void Viewer::showMessage(string word, bool click, int time) {
	int y = 20;//顯示起始上方座標
	int x = 32;//顯示起始左方座標
	int height;//顯示條高度
	int width;//顯示條寬度
	
	width = word.length()*3 + 2;

	height = 15;
	int wordHeight = height / 2;
	
	



	int grey = 127;
	int black = 0;
	int white = 255;
	int wordColor = 252;
	vector<vector<int>> message;
	message.resize(height);
	for (int i = 0; i < height; ++i) {
		message[i].resize(width);
	}
	for (int i = 0; i < height; ++i) {
		for (int j = 0; j < width; ++j) {
			message[i][j] = white;
		}
	}
	
	for (int i = 0; i < width; ++i) {
		message[0][i] = grey;
		message[height - 1][i] = grey;
	}
	for (int i = 0; i < height; ++i) {
		message[i][0] = grey;
		message[i][width - 1] = grey;
	}
	for (int i = 1; i < height-1; ++i) {
		message[i][1] = black;
		message[i][width - 2] = black;
	}
	
	
	for (int i = 0; i < height; ++i) {
		gotoyx(y + i, x);
		for (int j = 0; j < width; ++j) {
			SetColor(message[i][j]);
			cout << " ";
		}
		SetColor(black);
	}

	gotoyx(y + wordHeight, x + word.length());
	SetColor(wordColor);
	cout << word;
	SetColor(black);
	if (click) {
		voidClick();//空抓座標來當點擊確認
	}
	else {
		Sleep(time);
	}
	draw();
}
void Viewer::warn(Position p,int color) {//棋子座標轉畫面座標 畫框 色碼
	vector<Position> frame;
	for (int i = 0; i < 13; ++i) {
		frame.push_back(Position(p.y*7, p.x * 13 + i));
		frame.push_back(Position(p.y * 7+6, p.x * 13 + i));
	}
	for (int i = 0; i < 7; ++i) {
		frame.push_back(Position(p.y * 7+i, p.x * 13));
		frame.push_back(Position(p.y * 7 +i, p.x * 13 + 12));
	}
	SetColor(color);
	for (Position tmp : frame) {
		gotoyx(tmp.y, tmp.x);
		cout << " ";
	}
	cout << "\n";
	SetColor(0);
}
void Viewer::setTable(Board b) {
	vector<vector<Piece*>> m = b.m;
	for (int i = 0; i < 8; ++i) {
		for (int j = 0; j < 8; ++j) {
			if (m[i][j] != NULL) {
				table[i][j] = m[i][j]->getClass();
				owner[i][j] = m[i][j]->owner;
			}
			else {
				table[i][j] = Tab::empty;
				owner[i][j] = 3;
			}
		}
	}
}
void Viewer::clrTable() {
	table.clear();
}
void Viewer::setPmove(vector<Position> p) {
	pMove = p;
}
void Viewer::clrPmove() {
	pMove.clear();
}
void Viewer::draw() {
	drawing = true;
	gotoyx(0, 0);
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			int nDclass = table[i][j];//現在要畫的棋子
			for (int y = 0; y < 7; ++y) {
				for (int x = 0; x < 13; ++x) {
						if (pieceTab[nDclass][y][x] == 0) {			//決定畫白色或黑色
							if ((i + j) % 2 == 0) {
								screen[i * 7 + y][j * 13 + x] = 3;
							}
							else {
								screen[i * 7 + y][j * 13 + x] = 4;
							}
						}
					else if((pieceTab[nDclass][y][x] == 1)){
						if (owner[i][j]==1){
							screen[i * 7 + y][j * 13 + x] = 0;
						}
						else if(owner[i][j]==2){
							screen[i * 7 + y][j * 13 + x] = 1;
						}
					}
				}
			}
		}
	}
	for (Position p : pMove) {	//畫可以動位置的框
		for (int i = 0; i < 13; ++i)				//上
			screen[p.y * 7][p.x * 13 + i] = 2;
		for (int i = 0; i < 13; ++i)				//下
			screen[p.y * 7+6][p.x * 13 + i] = 2;
		for (int i = 0; i < 7; ++i)				//左
			screen[p.y *7+i][p.x * 13] = 2;
		for (int i = 0; i < 7; ++i)				//右
			screen[p.y * 7 + i][p.x * 13+12] = 2;
	}
	for (int i = 0; i < 56; ++i) {
		for (int j = 0; j < 104; ++j) {
			int s = screen[i][j];
			if (s == 0) {
				SetColor(255);
			}
			else if (s == 1) {
				SetColor(0);
			}
			else if (s == 2) {
				SetColor(238);
			}
			else if (s == 3) {
				SetColor(100);
			}
			else if (s == 4) {
				SetColor(43);
			}
			gotoyx(i, j);
			cout << " ";
			SetColor(0);
		}
	}
	drawing = false;

	/*for (vector<int> v : screen) {
		for (int s : v) {
			if (s == 0) {
				SetColor(255);
			}
			else if(s==1){
				SetColor(0);
			}
			else if (s == 2) {
				SetColor(238);
			}
			else if (s == 3) {
				SetColor(100);
			}
			else if (s == 4) {
				SetColor(43);
			}
			cout << " ";
			SetColor(0);
		}
			
		cout << "\n";
	}*/
}
void Viewer::promoteMenu() {
	vector<vector<int>>Pmenu;
	Pmenu.resize(9);
	for (int i = 0; i < 9; ++i)
		Pmenu[i].resize(54);
	for (int i = 0; i < 54; ++i) {
		Pmenu[0][i] = 1;
		Pmenu[8][i] = 1;
	}
	for (int i = 0; i < 9; ++i) {
		Pmenu[i][0] = 1;
		Pmenu[i][53] = 1;
	}
	for (int i = 1; i < 5; ++i) {
		for (int j = 0; j < 7; ++j) {
			for (int k = 0; k < 13; ++k) {
				Pmenu[j + 1][(i-1) * 13 + k + 1] = pieceTab[i][j][k];
			}
		}
	}
	for (int i = 0; i < 9; ++i) {
			gotoyx(4+i, 50);
		for (int j = 0; j < 54; ++j) {
			if (Pmenu[i][j] == 0) {
				SetColor(255);
			}
			else if (Pmenu[i][j] == 1) {
				SetColor(0);
			}
			gotoyx(4 + i, 50 + j);
			cout << " ";
			SetColor(0);
		}
	}
}

void Viewer::drawAdditional() {
	SetColor(121);
	for (int i = 0; i < 3; i++) {
		gotoyx(i, 104);
		for (int j = 0; j < 14; j++) {
			cout << " ";
		}
		cout << "\n";
	}
	gotoyx(3, 104);
	for (int i = 0; i < 5; i++) {
		cout << " ";
	}
	cout << "Undo";
	for (int i = 0; i < 5; i++) {
		cout << " ";
	}
	for (int i = 4; i < 7; i++) {
		gotoyx(i, 104);
		for (int j = 0; j < 14; j++) {
			cout << " ";
		}
		cout << "\n";
	}

	SetColor(249);
	gotoyx(7, 104);
	for (int i = 7; i < 10; i++) {
		gotoyx(i, 104);
		for (int j = 0; j < 14; j++) {
			cout << " ";
		}
		cout << "\n";
	}
	gotoyx(10, 104);
	for (int i = 0; i < 5; i++) {
		cout << " ";
	}
	cout << "Redo";
	for (int i = 0; i < 5; i++) {
		cout << " ";
	}

	for (int i = 11; i < 14; i++) {
		gotoyx(i, 104);
		for (int j = 0; j < 14; j++) {
			cout << " ";
		}
		cout << "\n";
	}

	SetColor(121);
	gotoyx(14, 104);
	for (int i = 14; i < 17; i++) {
		gotoyx(i, 104);
		for (int j = 0; j < 14; j++) {
			cout << " ";
		}
		cout << "\n";
	}
	gotoyx(17, 104);
	for (int i = 0; i < 3; i++) {
		cout << " ";
	}
	cout << "Surrender";
	for (int i = 0; i < 2; i++) {
		cout << " ";
	}

	for (int i = 18; i < 21; i++) {
		gotoyx(i, 104);
		for (int j = 0; j < 14; j++) {
			cout << " ";
		}
		cout << "\n";
	}

	SetColor(249);
	for (int i = 21; i < 24; i++) {
		gotoyx(i, 104);
		for (int j = 0; j < 14; j++) {
			cout << " ";
		}
		cout << "\n";
	}
	gotoyx(24, 104);
	for (int i = 0; i < 5; i++) {
		cout << " ";
	}
	cout << "Save";
	for (int i = 0; i < 5; i++) {
		cout << " ";
	}

	for (int i = 25; i < 28; i++) {
		gotoyx(i, 104);
		for (int j = 0; j < 14; j++) {
			cout << " ";
		}
		cout << "\n";
	}

	SetColor(121);
	for (int i = 28; i < 31; i++) {
		gotoyx(i, 104);
		for (int j = 0; j < 14; j++) {
			cout << " ";
		}
		cout << "\n";
	}
	gotoyx(31, 104);
	for (int i = 0; i < 5; i++) {
		cout << " ";
	}
	cout << "Menu";
	for (int i = 0; i < 5; i++) {
		cout << " ";
	}

	for (int i = 32; i < 35; i++) {
		gotoyx(i, 104);
		for (int j = 0; j < 14; j++) {
			cout << " ";
		}
		cout << "\n";
	}
}

void Viewer::drawOpening() {
	SetColor(7);
	int benchmark = 20;
	int counter = 0;
	string gameMode[5] = { "**                Start                **",
						   "**              Settings               **",
						   "**               Replay                **",
						   "**                Load                 **",
						   "**                Quit                 **" };
	vector<vector<char>> opening{ {'*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*'},
								  {'*', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '*'},
								  {'*', ' ', ' ', ' ', ' ', ' ', ' ', '*', '*', '*', '*', '*', '*', ' ', ' ', ' ', ' ', ' ', '*', '*', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '*'},
								  {'*', ' ', ' ', ' ', ' ', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', ' ', ' ', ' ', '*', '*', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '*'},
								  {'*', ' ', ' ', ' ', '*', '*', '*', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '*', '*', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '*'},
								  {'*', ' ', ' ', '*', '*', '*', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '*', '*', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '*', '*', '*', '*', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '*', '*', '*', '*', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '*', '*', '*', '*', ' ', ' ', ' ', ' ', '*'},
								  {'*', ' ', ' ', '*', '*', '*', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '*', '*', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '*', '*', '*', '*', '*', '*', '*', '*', ' ', ' ', ' ', ' ', '*', '*', '*', ' ', ' ', '*', '*', '*', ' ', ' ', ' ', '*', '*', '*', ' ', ' ', '*', '*', '*', ' ', ' ', '*'},
								  {'*', ' ', ' ', '*', '*', '*', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '*', '*', ' ', '*', '*', '*', '*', ' ', ' ', ' ', ' ', ' ', '*', '*', '*', ' ', ' ', ' ', ' ', '*', '*', '*', ' ', ' ', '*', '*', '*', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '*', '*', '*', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '*'},
								  {'*', ' ', ' ', '*', '*', '*', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '*', '*', '*', '*', ' ', ' ', '*', '*', ' ', ' ', ' ', ' ', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', ' ', ' ', ' ', ' ', '*', '*', '*', '*', '*', ' ', ' ', ' ', ' ', ' ', ' ', '*', '*', '*', '*', '*', ' ', ' ', ' ', ' ', '*'},
								  {'*', ' ', ' ', ' ', '*', '*', '*', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '*', '*', '*', ' ', ' ', ' ', ' ', '*', '*', ' ', ' ', ' ', '*', '*', '*', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '*', '*', '*', '*', '*', ' ', ' ', ' ', ' ', ' ', ' ', '*', '*', '*', '*', '*', ' ', ' ', '*'},
								  {'*', ' ', ' ', ' ', ' ', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', ' ', ' ', ' ', '*', '*', ' ', ' ', ' ', ' ', ' ', '*', '*', ' ', ' ', ' ', ' ', '*', '*', '*', '*', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '*', '*', ' ', ' ', ' ', '*', '*', ' ', ' ', ' ', ' ', '*', '*', ' ', ' ', ' ', '*', '*', ' ', ' ', ' ', '*'},
								  {'*', ' ', ' ', ' ', ' ', ' ', ' ', '*', '*', '*', '*', '*', '*', ' ', ' ', ' ', ' ', ' ', '*', '*', ' ', ' ', ' ', ' ', ' ', '*', '*', ' ', ' ', ' ', ' ', ' ', '*', '*', '*', '*', '*', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '*', '*', '*', '*', '*', ' ', ' ', ' ', ' ', ' ', ' ', '*', '*', '*', '*', '*', ' ', ' ', ' ', ' ', '*'},
								  {'*', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '*'},
								  {'*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*'} };
	for (int i = 0; i < 14; i++) {
		gotoyx(5 + i, 55);
		for (int j = 0; j < opening[0].size(); j++) {
			cout << opening[i][j];
		}
		cout << "\n";
	}

	gotoyx(benchmark, 65);
	cout << "*********************************************";
	gotoyx(benchmark + 1, 65);
	cout << "**                                         **";
	gotoyx(benchmark + 2, 65);
	cout << "**        Welcome to The Chess Game        **";
	gotoyx(benchmark + 3, 65);
	cout << "**                                         **";
	gotoyx(benchmark + 4, 65);
	cout << "**                                         **";
	gotoyx(benchmark + 5, 65);
	cout << "**     Author: Guoup 24, Xitsu, HsinYi     **";
	gotoyx(benchmark + 6, 65);
	cout << "**                                         **";
	gotoyx(benchmark + 7, 65);
	cout << "*********************************************";
	for (int i = benchmark + 9; i < benchmark + 30; i += 5) {
		gotoyx(i, 67);
		cout << "*****************************************";
		gotoyx(i + 1, 67);
		cout << "**                                     **";
		gotoyx(i + 2, 67);
		cout << gameMode[counter];
		gotoyx(i + 3, 67);
		cout << "**                                     **";
		gotoyx(i + 4, 67);
		cout << "*****************************************";
		counter++;
	}
}

void Viewer::drawReplayButtons() {
	SetColor(121);
	for (int i = 0; i < 3; i++) {
		gotoyx(i, 104);
		for (int j = 0; j < 14; j++) {
			cout << " ";
		}
	}
	gotoyx(3, 104);
	cout << "Previous  Step";
	for (int i = 4; i < 7; i++) {
		gotoyx(i, 104);
		for (int j = 0; j < 14; j++) {
			cout << " ";
		}
	}

	SetColor(249);
	gotoyx(7, 104);
	for (int i = 7; i < 10; i++) {
		gotoyx(i, 104);
		for (int j = 0; j < 14; j++) {
			cout << " ";
		}
	}
	gotoyx(10, 104);
	for (int i = 0; i < 3; i++) {
		cout << " ";
	}
	cout << "Next Step";
	for (int i = 0; i < 2; i++) {
		cout << " ";
	}

	for (int i = 11; i < 14; i++) {
		gotoyx(i, 104);
		for (int j = 0; j < 14; j++) {
			cout << " ";
		}
	}

	SetColor(121);
	gotoyx(14, 104);
	for (int i = 14; i < 17; i++) {
		gotoyx(i, 104);
		for (int j = 0; j < 14; j++) {
			cout << " ";
		}
	}
	gotoyx(17, 104);
	for (int i = 0; i < 2; i++) {
		cout << " ";
	}
	cout << "End Replay";
	for (int i = 0; i < 2; i++) {
		cout << " ";
	}
	for (int i = 18; i < 21; i++) {
		gotoyx(i, 104);
		for (int j = 0; j < 14; j++) {
			cout << " ";
		}
	}
}