#include "GameManager.h"
using namespace std::this_thread;
GameManager::GameManager() {
	
	current_player = 0;
	AI = 0;
}
void GameManager::settings() {
	viewer.Setting(AI,ForL);
	while (true) {
		Position tmp = getClick();
		if (tmp.y >= 8 && tmp.y <= 12 && tmp.x >= 40 && tmp.x <= 46) {
			AI = 0;
		}
		else if (tmp.y >= 8 && tmp.y <= 12 && tmp.x >= 47 && tmp.x <= 53) {
			AI = 1;
		}
		if (tmp.y >= 18 && tmp.y <= 22 && tmp.x >= 27 && tmp.x <= 33) {
			ForL = 1;
		}
		else if (tmp.y >= 18 && tmp.y <= 22 && tmp.x >= 34 && tmp.x <= 40) {
			ForL = 2;
		}
		if (tmp.y >= 30 && tmp.y <= 32 && tmp.x >= 10 && tmp.x <= 18) {
			clearS();
			break;
		}
		viewer.Setting(AI, ForL);
	}
}

void GameManager::init() {
	ShowConsoleCursor(false);
	while (true) {
		viewer.drawOpening();
		Position pos; //4, 41
		pos = getClick();
		if (pos.y >= 29 && pos.y <= 33 && pos.x >= 67 && pos.x <= 108) { // Mode Selection
			start();
		}
		else if (pos.y >= 34 && pos.y <= 38 && pos.x >= 67 && pos.x <= 108) { // Settings
			settings();
		}
		else if (pos.y >= 39 && pos.y <= 44 && pos.x >= 67 && pos.x <= 108) { // Replay
			replay();
		}
		else if (pos.y >= 45 && pos.y <= 50 && pos.x >= 67 && pos.x <= 108) { // Load
			load();
		}
		else if (pos.y >= 51 && pos.y <= 55 && pos.x >= 67 && pos.x <= 108) { // Quit
			quit();
		}
		clearS();
	}

}
void countDown(GameManager& gamemanager) {
	Sleep(500);
	while (gamemanager.players[gamemanager.current_player]->count_T > 0) {
		while(gamemanager.viewer.drawing) {
			sleep_for(std::chrono::milliseconds(30));
		}
		--gamemanager.players[gamemanager.current_player]->count_T;
		gamemanager.viewer.drawTime(0, gamemanager.players[0]->count_T);
		gamemanager.viewer.drawTime(1, gamemanager.players[1]->count_T);
		sleep_for(std::chrono::duration<int>(1));
	}
}
void GameManager::start() {
	board.init();
	clearS();
	viewer.setTable(board);
	viewer.draw();
	if (AI == 0) {//沒有電腦 啟動計時器
		players[0] = new HumanPlayer(1);
		players[1] = new HumanPlayer(2);
		viewer.drawTime(0, players[0]->count_T);
		viewer.drawTime(1, players[1]->count_T);
		std::thread TimeCount(countDown, std::ref(*this));
		TimeCount.detach();
	}

	else if (AI == 1) {
		if (ForL == 1) {
			players[0] = new HumanPlayer(1);
			players[1] = new basicAiPlayer(2);
		}
		else if (ForL == 2) {
			players[0] = new basicAiPlayer(1);
			players[1] = new HumanPlayer(2);
		}
	}
	while (true)
	{
		viewer.drawAdditional();
		if (current_player == 0) {
			int res = players[0]->lose(board, viewer);
			if (res==1) {
				viewer.warn(players[0]->kPos, 85);
				viewer.showMessage("Black wins", true, 0);
				break;
			}
			else if (res == 2) {
				viewer.showMessage("Ties", true, 0);
				break;
			}
			players[0]->OnMove(board, viewer);
			if (board.someoneSurrender) {
				board.someoneSurrender = false;
				break;
			}
			if (board.callbackMenu) {
				board.callbackMenu = false;
				break;
			}
			players[0]->OnPromote(board, viewer);
			if (players[0]->tieCheck(board, viewer)) {
				viewer.showMessage("Ties", true, 0);
				break;
			}
			current_player = 1;
		}
		else if (current_player == 1) {
			int res = players[1]->lose(board, viewer);
			if (res == 1) {
				viewer.warn(players[1]->kPos, 85);
				viewer.showMessage("White wins", true, 0);
				break;
			}
			else if (res == 2) {
				viewer.showMessage("Ties", true, 0);
				break;
			}
			players[1]->OnMove(board, viewer);
			if (board.someoneSurrender) {
				board.someoneSurrender = false;
				break;
			}
			if (board.callbackMenu) {
				board.callbackMenu = false;
				break;
			}
			players[1]->OnPromote(board, viewer);
			if (players[1]->tieCheck(board, viewer)) {
				viewer.showMessage("Ties", true, 0);
				break;
			}
			current_player = 0;
		}
		if (players[0]->count_T == 0) {
			viewer.showMessage("Time out Black wins", true, 0);
			break;
		}
		else if (players[1]->count_T == 0) {
			viewer.showMessage("Time out White wins", true, 0);
			break;
		}
		Sleep(40);
		board.record();//每步走完紀錄棋盤

	}
	players[0]->count_T = 0;
	players[1]->count_T = 0;
	board.saveReplay();
}

void GameManager::quit() {
	HWND hwmd;
	hwmd = GetConsoleWindow();
	PostMessage(hwmd, WM_CLOSE, 0, 0);
}
void GameManager::load() {
	clearS();
	gotoyx(0, 0);
	string file;
	std::cout << "Enter replay file name:";
	std::cin >> file;
	ifstream ifs(file);
	string s;
	ifs >> s;
	
	stringstream ss(s);

	vector<vector<pair<int, int>>> tmp;
	tmp.resize(8);
	for (int i = 0; i < 8; ++i)
		tmp[i].resize(8);

	for (int i = 0; i < 8; ++i) {
		for (int j = 0; j < 8; ++j) {
			char chr;
			ss >> chr;
			tmp[i][j].first = chr - '0';
			ss >> chr;
			tmp[i][j].second = chr - '0';
		}
	}
	board.recreate(tmp);

	int v1, v2, v3, v4;
	board.steps.clear();
	while (ifs >> v1 >> v2 >> v3 >> v4) {
		board.steps.push_back(pair<Position, Position>(Position(v1, v2), Position(v3, v4)));
	}
	int tmpSize = board.steps.size();
	if (tmpSize % 2 == 0) {
		current_player = 0;
	}
	else {
		current_player = 1;
	}

	//執行start內容
	clearS();
	viewer.setTable(board);
	viewer.draw();
	if (AI == 0) {//沒有電腦 啟動計時器
		players[0] = new HumanPlayer(1);
		players[1] = new HumanPlayer(2);
		viewer.drawTime(0, players[0]->count_T);
		viewer.drawTime(1, players[1]->count_T);
		std::thread TimeCount(countDown, std::ref(*this));
		TimeCount.detach();
	}

	else if (AI == 1) {
		if (ForL == 1) {
			players[0] = new HumanPlayer(1);
			players[1] = new basicAiPlayer(2);
		}
		else if (ForL == 2) {
			players[0] = new basicAiPlayer(1);
			players[1] = new HumanPlayer(2);
		}
	}
	while (true)
	{
		viewer.drawAdditional();
		if (current_player == 0) {
			int res = players[0]->lose(board, viewer);
			if (res == 1) {
				viewer.warn(players[0]->kPos, 85);
				viewer.showMessage("Black wins", true, 0);
				break;
			}
			else if (res == 2) {
				viewer.showMessage("Ties", true, 0);
				break;
			}
			players[0]->OnMove(board, viewer);
			if (board.someoneSurrender) {
				board.someoneSurrender = false;
				break;
			}
			if (board.callbackMenu) {
				board.callbackMenu = false;
				break;
			}
			players[0]->OnPromote(board, viewer);
			if (players[0]->tieCheck(board, viewer)) {
				viewer.showMessage("Ties", true, 0);
				break;
			}
			current_player = 1;
		}
		else if (current_player == 1) {
			int res = players[1]->lose(board, viewer);
			if (res == 1) {
				viewer.warn(players[1]->kPos, 85);
				viewer.showMessage("White wins", true, 0);
				break;
			}
			else if (res == 2) {
				viewer.showMessage("Ties", true, 0);
				break;
			}
			players[1]->OnMove(board, viewer);
			if (board.someoneSurrender) {
				board.someoneSurrender = false;
				break;
			}
			if (board.callbackMenu) {
				board.callbackMenu = false;
				break;
			}
			players[1]->OnPromote(board, viewer);
			if (players[1]->tieCheck(board, viewer)) {
				viewer.showMessage("Ties", true, 0);
				break;
			}
			current_player = 0;
		}
		if (players[0]->count_T == 0) {
			viewer.showMessage("Time out Black wins", true, 0);
			break;
		}
		else if (players[1]->count_T == 0) {
			viewer.showMessage("Time out White wins", true, 0);
			break;
		}
		Sleep(40);
		board.record();//每步走完紀錄棋盤

	}
	players[0]->count_T = 0;
	players[1]->count_T = 0;
	board.saveReplay();
}

void GameManager::replay() {
	clearS();
	gotoyx(0, 0);
	string file;
	std::cout << "Enter replay file name:";
	std::cin >> file;
	ifstream ifs(file);
	string s;
	ifs >> s;
	vector<vector<vector<pair<int, int>>>> v;
	vector<vector<pair<int, int>>> tmp;
	tmp.resize(8);
	for (int i = 0; i < 8; ++i)
		tmp[i].resize(8);
	stringstream ss(s);
	while (!ss.eof()) {
		for (int i = 0; i < 8; ++i) {
			for (int j = 0; j < 8; ++j) {
				char chr;
				ss >> chr;
				tmp[i][j].first = chr - '0';
				ss >> chr;
				tmp[i][j].second = chr - '0';
			}
		}
		v.push_back(tmp);
	}
	
	viewer.drawReplayButtons();
	int ind = 0;
	while (true) {
		board.recreate(v[ind]);
		viewer.setTable(board);
		viewer.draw();
		Position pos = getClick();
		int tableY = pos.y / 7;
		int tableX = pos.x / 13;
		if (tableX == 8) {
			if (tableY == 0) {
				if (ind==0) {
					viewer.showMessage("First step", false, 500);
				}
				else {
					ind--;
				}
			}
			else if (tableY == 1) {
				if (ind == v.size()-2) {
					viewer.showMessage("Last step", false, 500);
				}
				else {
					ind++;
				}
			}
			else if (tableY == 2) {
				break;
			}
		}
	}
}