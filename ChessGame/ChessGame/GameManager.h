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
	int AI;//0��⪱�a 1²��q��
	int ForL;//����� 1���� 2����
	Board board;
	Viewer viewer;
	void start();
	void settings();
	void replay();
	void load();
	void quit();
	void init();
};
void countDown(GameManager&);//���ư���� ����I�s
