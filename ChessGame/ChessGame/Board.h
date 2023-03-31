#include <vector>
#include <string>
#include <Windows.h>
#include <ctime>
#include <fstream>
#pragma warning(disable : 4996) 
using std::vector;
using std::string;
using std::pair;
enum Tab//棋子的enum
{
	pawn = 0,
	rock,
	knight,
	bishop,
	queen,
	king,
	empty
};
class Position {
public:
	Position();
	Position(int, int);
	Position operator+(Position);
	bool operator==(Position);
	bool operator!=(Position);
	bool inRange();//檢查是否超出棋盤
	int y, x;
};
class Board; class Piece {
public:
	int owner;//3代表沒有   1 代表白色 2代表黑色
	int pClass;
	Position pos;
	virtual vector<Position> possibleMove(Board& m, Position);
	virtual int getClass();
	virtual void change(Board&, Position);
};
class Board {
public:
	Board();
	//~Board();
	bool someoneSurrender;
	bool callbackMenu;
	int peaceMove;//沒吃子的回合數
	int pieceNum();//計算場上棋子數量
	void recycle();
	int current_player;
	string name;//給本盤棋局定名稱
	void saveReplay();
	void recreate(vector<vector<pair<int, int>>>);//重建棋盤
	vector<vector<vector<pair<int,int>>>> stepRecord;//紀錄棋盤每一步後狀況
	vector<vector<vector<pair<int, int>>>> redoStepRecord;//紀錄undo前來redo
	void copyBoard(Board);//把參數的board複製進
	vector<pair<Position,Position>> steps;//紀錄每一步從哪到哪
	vector<pair<Position, Position>> redoSteps;//紀錄每一步從哪到哪
	void record();//將棋盤存進stepRecord
	vector<vector<Piece*>> m;//棋盤
	void init();
	Piece* createPiece(int);
	bool isVaild(Position,int,bool);//檢查該格是否被攻擊 給king檢查王車易位跳過檢查對方的king避免迴圈
	void save();
};



class Pawn : public Piece {
	vector<Position> possibleMove( Board& m,Position pos);
	void change(Board&, Position);
	int getClass();
};
class Rock : public Piece {
	vector<Position> possibleMove( Board& m,Position pos);
	void change(Board&, Position);
	int getClass();
};
class Knight : public Piece {
	vector<Position> possibleMove( Board& m,Position pos);
	void change(Board&, Position);
	int getClass();
};
class Bishop : public Piece {
	vector<Position> possibleMove( Board& m,Position pos);
	void change(Board&, Position);
	int getClass();
};
class Queen : public Piece {
	vector<Position> possibleMove( Board& m,Position pos);
	void change(Board&, Position);
	int getClass();
};
class King : public Piece {
	vector<Position> possibleMove( Board& m,Position pos);
	void change(Board&, Position);
	int getClass();
};