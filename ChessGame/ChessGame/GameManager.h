#include <thread>
#include <chrono>
#include <fstream>
#include <sstream>
#include "Player.h"
class GameManager {
public:
	GameManager();
	Player* players[2];
	int current_player;
	int AI;//0對手玩家 1簡單電腦
	int ForL;//先後手 1走白 2走黑
	Board board;
	Viewer viewer;
	void start();
	void settings();
	void replay();
	void load();
	void quit();
	void init();
};
void countDown(GameManager&);//給副執行緒 不能呼叫
