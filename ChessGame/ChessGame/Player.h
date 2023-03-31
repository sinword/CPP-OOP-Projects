#include "Viewer.h"
#include <cstdlib>
#include <ctime>
#include <Windows.h>
class Player {
public:
	Player(int);
	int owner;
	int count_T;//剩餘時間(秒)
	Position kPos;//國王位置
	virtual bool tieCheck(Board&, Viewer&) = 0;
	virtual void OnMove(Board&,Viewer&) = 0;
	virtual void OnPromote(Board&, Viewer&) = 0;
	virtual int lose(Board&, Viewer&) = 0;
};
class HumanPlayer : public Player{
public:
	HumanPlayer(int);
	void OnMove(Board&,Viewer&);
	void OnPromote(Board&, Viewer&);
	bool tieCheck(Board&, Viewer&);
	int lose(Board&, Viewer&);

};
class basicAiPlayer :public Player {
public:
	basicAiPlayer(int);
	void OnMove(Board&, Viewer&);
	void OnPromote(Board&, Viewer&);
	bool tieCheck(Board&, Viewer&);
	int lose(Board&, Viewer&);
	vector<pair<Position, Position>> moveableCombine(Board&, Viewer&);
};
Position getClick();