#include <Windows.h>
#include <iostream>
#include <string>
#include <conio.h>
#include <vector>

#define width 120
#define height 32
using namespace std;

class Viewer {
public:
	string screen[height][width];
	int color[height][width];
	Viewer();
	void setWord(int y, int x, string word);
	void setNdraw(int y, int x, string word);
	void draw();
	void clear();
	void clearColor();
	void dHorse(int,int,int);
	void cOneline(int,int color=7);//¦æ¼Æ ÃC¦â
	string getInput(int, int, int);
};
void SetColor(int);
void fontSize(int w, int h);
void gotoyx(int ypos, int xpos);
