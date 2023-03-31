#include <vector>
#include <string>
#include <Windows.h>
#include <ctime>
#include <fstream>
#pragma warning(disable : 4996) 
using std::vector;
using std::string;
using std::pair;
enum Tab//�Ѥl��enum
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
	bool inRange();//�ˬd�O�_�W�X�ѽL
	int y, x;
};
class Board; class Piece {
public:
	int owner;//3�N��S��   1 �N��զ� 2�N��¦�
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
	int peaceMove;//�S�Y�l���^�X��
	int pieceNum();//�p����W�Ѥl�ƶq
	void recycle();
	int current_player;
	string name;//�����L�ѧ��w�W��
	void saveReplay();
	void recreate(vector<vector<pair<int, int>>>);//���شѽL
	vector<vector<vector<pair<int,int>>>> stepRecord;//�����ѽL�C�@�B�᪬�p
	vector<vector<vector<pair<int, int>>>> redoStepRecord;//����undo�e��redo
	void copyBoard(Board);//��Ѽƪ�board�ƻs�i
	vector<pair<Position,Position>> steps;//�����C�@�B�q�����
	vector<pair<Position, Position>> redoSteps;//�����C�@�B�q�����
	void record();//�N�ѽL�s�istepRecord
	vector<vector<Piece*>> m;//�ѽL
	void init();
	Piece* createPiece(int);
	bool isVaild(Position,int,bool);//�ˬd�Ӯ�O�_�Q���� ��king�ˬd����������L�ˬd��誺king�קK�j��
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