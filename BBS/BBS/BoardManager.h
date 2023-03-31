#include <cstdlib>
#include <ctime>
#include "Board.h"
#include "Viewer.h"
#include "User.h"
#include "addtionalTool.h"
#include "mail.h"
#include <sstream>
#include <thread>
class BoardManager {
public:
	enum BoardState
	{
		LOGIN,
		MENU,
		PROFILE,
		MAIL,
		POSTHISTORY,
		GAME,
		SELECT_BOARD,
		BOARD,
		POST
	};
	BoardState state;
	vector<User*> users;
	vector<Board> boards;

	int current_user;
	int current_board;
	int current_post;
	MailSystem ms;
	Viewer viewer;
	BoardManager();
	void refresh();//刷新清單
	void login();
	void signup();
	void menu();
	void selectboard();
	void deleteboard(int);
	void board();
	void post();
	void profile();
	void mail();
	void posthistory();
	void showMail(Mail);
	void game();
	void afk();
	void race();
	string editMode(string);//編輯放棄回傳(char)24
	string editexist(string, string);
};
