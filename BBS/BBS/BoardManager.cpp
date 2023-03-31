#include "BoardManager.h"
#include <thread>
using namespace std::this_thread;
using namespace std;
BoardManager::BoardManager() {
	state = BoardState::LOGIN;
	refresh();
	while (true) {
		refresh();
		switch (state)
		{
		case BoardManager::LOGIN:
			login();
			break;
		case BoardManager::MENU:
			menu();
			break;
		case BoardState::PROFILE:
			profile();
			break;
		case BoardState::MAIL:
			mail();
			break;
		case BoardState::POSTHISTORY:
			posthistory();
			break;
		case BoardState::GAME:
			game();
			break;
		case BoardManager::SELECT_BOARD:
			selectboard();
			break;
		case BoardManager::BOARD:
			board();
			break;
		case BoardManager::POST:
			post();
			break;
		default:
			break;
		}
	}
}
void BoardManager::game() {
	int showlistnum = 15;
	viewer.clear();
	vector<string> MENU;
	MENU.push_back("賽馬");
	MENU.push_back("掛機");
	MENU.push_back("MAIL");
	

	int index = 0;
	int cursor = 0;
	while (true) {
		for (int i = 0; i + index < MENU.size() && i < showlistnum; ++i) {
			gotoyx(10 + i, 20);
			viewer.cOneline(10 + i);
			viewer.setNdraw(10 + i, 20, MENU[i + index]);
		}
		viewer.setNdraw(10 + cursor, 18, "->");
		char chr = _getch();
		if (chr == -32) {//方向鍵
			chr = _getch();
			if (chr == 80) {//down arrow
				if (index + cursor < MENU.size() - 1) {
					if (cursor < showlistnum - 1) {
						cursor += 1;
					}
					else if (cursor == showlistnum - 1) {
						index += 1;
					}

				}
			}
			else if (chr == 72) {//up arrow
				if (cursor > 0) {
					cursor -= 1;
				}
				else if (cursor == 0 && index > 0) {
					index -= 1;
				}
			}
			else if (chr == 77) {//right arrow
				if (cursor + index == 0) {
					race();
					viewer.clear();
				}
				else if (cursor + index == 1) {
					afk();
					viewer.clear();
				}
			}
			else if (chr == 75) {//left arrow
				state = BoardState::MENU;
				return;
			}
		}

	}
	
	
}
void BoardManager::race() {
	viewer.clear();
	while (true) {
		int hP[] = { 0,0,0,0 };
		viewer.dHorse(2, 5, 10);
		viewer.dHorse(7, 5, 20);
		viewer.dHorse(12, 5, 30);
		viewer.dHorse(17, 5, 40);
		SetColor(255);
		for (int i = 0; i < 20; ++i) {
			gotoyx(2 + i, 60);
			cout << " ";
		}
		SetColor(7);
		viewer.setNdraw(23, 5, "參加(p)退出(esc)");
		viewer.setNdraw(24, 5, "錢: " + to_string(users[current_user]->money));
		char chr;
		int num,val;
		while (chr = _getch()) {
			if (chr == 'p') {
				viewer.cOneline(23);
				viewer.setNdraw(23,5,"要下注的號碼: ");
				cin >> num;
				while (true) {
					viewer.cOneline(23);
					viewer.setNdraw(23, 5, "要下注的金額: ");
					cin >> val;
					if (users[current_user]->money < val) {
						viewer.cOneline(23);
						viewer.setNdraw(23, 5, "餘額不足 ");
						Sleep(1500);
						continue;
					}
					users[current_user]->money -= val;
					viewer.setNdraw(24, 5, "錢: " + to_string(users[current_user]->money));
					break;
				}
				break;
			}
			else if (chr == 27) {
				viewer.clear();
				return;
			}
		}
		srand(time(NULL));
		int wH[] = { 0,0,0,0 };
		while (true) {
			bool fflag = false;
			viewer.cOneline(23);
			gotoyx(23, 5);
			for (int i = 0; i < 4; ++i) {
				if (rand() % 4 == 0) {
					hP[i]++;
				}
				if (hP[i] == 50) {
					fflag = true;
					wH[i] = 1;
					cout << to_string(i) + "號 ";
				}
			}
			SetColor(255);
			for (int i = 0; i < 20; ++i) {
				gotoyx(2 + i, 60);
				cout << " ";
			}
			SetColor(7);
			for (int i = 0; i < 20; ++i) {
				for (int j = 0; j < 4; ++j) {
					for (int l = 0; l < 5; l++) {
						gotoyx(2 + j * 5+l, 4 + hP[j]);
						cout << " ";
					}
				}
			}
			viewer.dHorse(2, 5+hP[0], 10);
			viewer.dHorse(7, 5+hP[1], 20);
			viewer.dHorse(12, 5+hP[2], 30);
			viewer.dHorse(17, 5+hP[3], 40);
			if (fflag) {
				break;
			}
		}
		int count = 0;
		for (int i : wH) {
			if (i)
				count++;
		}
		gotoyx(23, 5);
		if (wH[num]) {
			users[current_user]->money += val * 10 / count;
			cout << "贏了 你得到 " << val * 10 / count << "元";
		}
		else {
			cout << "你輸了";
		}
		users[current_user]->save();
		Sleep(2000);
		viewer.clear();
		
	}
}
void intputALT(bool& flag) {
	char chr;
	while (chr = _getch()) {
		if (chr == 27) {
			flag = true;
			return;
		}
	}
}
void BoardManager::afk() {
	bool flag = false;
	std::thread inputDec(intputALT, std::ref(flag));
	inputDec.detach();
	viewer.clear();
	viewer.setNdraw(5, 30, string(20, '='));
	for (int i = 0; i < 10; ++i) {
		gotoyx(6 + i, 30);
		cout << "=";
		gotoyx(6 + i, 49);
		cout << "=";
	}
	viewer.setNdraw(17, 30, string(20, '='));
	vector<string> books;
	books.push_back("工程數學");
	books.push_back("線性代數");
	books.push_back("微積分");
	books.push_back("離散數學");
	viewer.setNdraw(23, 5, "退出(esc)");
	while (true) {
		if (flag) {
			return;
		}
		srand(time(NULL));
		int nB = rand() % books.size();
		viewer.setNdraw(10, 36, "        ");
		viewer.setNdraw(10, 36, books[nB]);
		viewer.setNdraw(22, 5, "錢: " + to_string(users[current_user]->money));
		int consume = 15 + (rand() % 10);
		//int consume = 30;
		int otime = consume;
		while (true) {
			if (flag) {
				return;
			}
			viewer.setNdraw(9, 10, "閱讀時間");
			viewer.setNdraw(10, 10, "(" + to_string(consume) + "/" + to_string(otime) + ")");
			gotoyx(11, 7);
			cout << "<";
			gotoyx(11, 23);
			cout << ">";
			float unit = (float)otime / 15;
			if (unit == 0) {
				unit = 1;
			}
			float nT = otime - consume;
			int part = nT / unit;
			gotoyx(11, 23 - part);
			cout << string(part, '=');
			sleep_for(std::chrono::milliseconds(980));
			if (consume == 0) {
				break;
			}
			consume--;
		}
		users[current_user]->money += rand() % 5 + 30;
		users[current_user]->save();
		viewer.setNdraw(22, 5, "錢: " + to_string(users[current_user]->money));
		viewer.setNdraw(9, 10, "換書中  ");
		viewer.setNdraw(10, 10, string(8, ' '));
		int times = 0;
		for (int i = 0; i < 12; ++i) {
			if (flag) {
				return;
			}
			switch (times)
			{
			case 0:
				viewer.setNdraw(10, 10, "●        ");
				times++;
				break;
			case 1:
				viewer.setNdraw(10, 10, "●  ●    ");
				times++;
				break;
			case 2:
				viewer.setNdraw(10, 10, "●  ●  ●");
				times = 0;
				break;
			}
			sleep_for(std::chrono::milliseconds(333));
		}
		viewer.setNdraw(10, 10, "          ");
		viewer.setNdraw(11, 7, string(20,' '));

	}
}

void BoardManager::deleteboard(int index) {
	string tmp = ".\\Board\\" + boards[index].name;
	
	rename(tmp.c_str(), (boards[index].name+ "-" + getCleanTimeString()).c_str());
	boards[index].name = "!" + boards[index].name;
	ofstream ofs;
	ofs.open(".\\Board\\boardlist.txt");
	ofs.clear();
	for (Board b : boards) {
		ofs << b.name << "\n";
	}
	ofs.close();
	refresh();
}
void BoardManager::posthistory() {
	viewer.clear();
	int showlistnum = 15;
	int strY = 5;
	int index = 0;
	int cursor = 0;
	User* nU = users[current_user];
	vector<Post> posts;
	for (Board b : boards) {
		for (Post p : b.postlist) {
			if (p.account == nU->account) {
				posts.push_back(p);
			}
		}
	}
	viewer.setNdraw(23, 10, "刪文(d)");
	
	
	while (true) {
		for (int i = 0; i + index < posts.size() && i < showlistnum; ++i) {
			gotoyx(strY + i, 20);
			viewer.cOneline(strY + i);
			viewer.setNdraw(strY + i, 20, posts[i + index].board+"\\"+posts[i + index].Title);
		}
		viewer.setNdraw(strY + cursor, 18, "->");
		char chr = _getch();
		if (chr == -32) {//方向鍵
			chr = _getch();
			if (chr == 80) {//down arrow
				if (index + cursor < posts.size() - 1) {
					if (cursor < showlistnum - 1) {
						cursor += 1;
					}
					else if (cursor == showlistnum - 1) {
						index += 1;
					}

				}
			}
			else if (chr == 72) {//up arrow
				if (cursor > 0) {
					cursor -= 1;
				}
				else if (cursor == 0 && index > 0) {
					index -= 1;
				}
			}
			else if (chr == 77) {//right arrow
			}
			else if (chr == 75) {//left arrow
				state = BoardState::MENU;
				return;
			}
		}
		else if (chr == 'd') {
			ifstream ifs;
			ifs.open(".\\Board\\" + posts[index + cursor].board + "\\postlist.txt");
			vector<string> bl;
			string tmp;
			while (ifs >> tmp) {
				bl.push_back(tmp);
			}
			ifs.close();
			ofstream ofs;
			ofs.open(".\\Board\\" + posts[index + cursor].board + "\\postlist.txt");
			ofs.clear();
			for (string s : bl) {
				if (s == posts[index + cursor].ID) {
					ofs << "!" + s << "\n";
				}
				else {
					ofs << s <<"\n";
				}
			}
			ofs.close();
			posts.erase(posts.begin() + index + cursor);
			refresh();
			viewer.setNdraw(23, 10, "刪文成功");
			Sleep(2000);
			viewer.clear();
			viewer.setNdraw(23, 10, "刪文(d)");
		}
		
	}
}
string BoardManager::editMode(string work) {
	viewer.clear();
	int textWidth = 40;
	int strY = 1;
	int nY = 0;
	int strX = 5;
	int nX = 0;
	vector<string> opt;
	vector<int> optnX;
	viewer.setNdraw(0, strX, work);
	gotoyx(1, strX);
	char c;
	//string line(textWidth, ' ');
	opt.push_back("");
	optnX.push_back(nX);
	while (c = _getch()) {
		string tmp = "";
		tmp += c;
		if (c == -32) {//方向鍵
		c = _getch();
		if (c == 80) {//down arrow
			if (nY + 1 < opt.size()) {
				nY++;
				if (opt[nY].length() <= nX) {
					nX = optnX[nY];
				}
			}
		}
		else if (c == 72) {//up arrow
			if (nY > 0) {
				nY--;
				if (opt[nY].length() <= nX) {
					nX = optnX[nY];
				}
			}
		}
		else if (c == 77) {//right arrow
			if (nX < opt[nY].length() - 2) {
				if (opt[nY][nX]&0x80) {
					nX += 2;
				}
				else {
					nX++;
				}
				
			}
			else if (nX < opt[nY].length() - 1) {
				nX++;
			}
			else if (nX == opt[nY].length() - 1) {
				if (opt.size() > nY + 1) {
					nY++;
					nX = 0;
				}
			}
		}
		else if (c == 75) {//left arrow
			if (nX > 1) {
				if (opt[nY][nX - 2] & 0x80 && opt[nY][nX-1]&0x80) {
					nX -= 2;
				}
				else {
					nX -= 1;
				}
			}
			else if (nX > 0) {
				nX--;
			}
			else if (nX == 0) {
				if (nY > 0) {
					nY--;
					nX = optnX[nY];
				}
			}
		}

		}else if (c & 0x80) {
			if (opt[nY].length() + 1 >= textWidth) {
				optnX[nY] = nX;
				nY++;
				nX = 0;
				optnX.insert(optnX.begin() + nY, nX);
				opt.insert(opt.begin() + nY, "");
			}
			tmp += _getch();
			opt[nY].insert(nX, tmp);
			nX += 2;
		}
		else if (c == 13) {//enter
			optnX[nY] = nX;
			nY++;
			nX = 0;
			optnX.insert(optnX.begin()+nY,nX);
			opt.insert(opt.begin()+nY,"");
		}
		else if (c == 8) {//backspace
			if (nX > 1) {//可能有中文
				if (opt[nY][nX - 2] & 0x80) {
					opt[nY].erase(nX - 2, 2);
					nX -= 2;
				}
				else {
					opt[nY].erase(nX - 1);
					nX -= 1;
				}
			}
			else if (nX == 1) {
				opt[nY].erase(nX - 1);
				nX -= 1;
			}
			else if (nX == 0) {
				if (nY > 0) {
					if (opt[nY].length() == 0) {
						opt.erase(opt.begin() + nY);
						optnX.erase(optnX.begin() + nY);
					}
					nY--;
					nX = optnX[nY];
				}
			}
		}
		else if (c == 27) {//esc
			viewer.setNdraw(13, strX,"發送/停留/放棄(Y/S/N)");
			string output = "";
			while (c = _getch()) {
				bool fflag = false;
				switch (c)
				{
					case 'Y':
						for (int i = 0; i < opt.size() - 1; ++i) {
							output += opt[i];
							output += "\n";
						}
						output += opt[opt.size() - 1];
						return output;
					case 'S':
						fflag = true;
						break;
					case 'N':
						return ""+(char)24;
					default:
						continue;
				}
				if (fflag) {
					viewer.clear();
					viewer.setNdraw(0, strX, work);
					for (int i = 0; i < opt.size(); ++i) {
						gotoyx(strY + i, strX);
						cout << opt[i];
					}
					gotoyx(nY, nX);
					break;
				}

				
			}
			
		}
		else if (isalnum(c)||c==32) {
			if (opt[nY].length() >= textWidth) {
				optnX[nY] = nX;
				nY++;
				nX = 0;
				optnX.insert(optnX.begin() + nY, nX);
				opt.insert(opt.begin() + nY, "");
			}
			opt[nY].insert(nX, tmp);
			nX += 1;
		}
		for (int i = 0; i < opt.size();++i) {
			string otmp = opt[i] + string(textWidth - opt[i].length(), ' ');
			gotoyx(strY + i, strX);
			cout << otmp;
		}
		gotoyx(strY + nY, strX + nX);
	}
	return "";
}
string BoardManager::editexist(string work, string load) {
	viewer.clear();
	int textWidth = 40;
	int strY = 1;
	int nY = 0;
	int strX = 5;
	int nX = 0;
	vector<string> opt;
	vector<int> optnX;
	stringstream ss(load);
	string lopt;
	int lY = 0;
	while (getline(ss, lopt)) {
		gotoyx(strY+lY, strX);
		cout << lopt;
		lY++;
		opt.push_back(lopt);
		optnX.push_back(lopt.length());
	}
	viewer.setNdraw(0, strX, work);
	gotoyx(1, strX);
	char c;
	while (c = _getch()) {
		string tmp = "";
		tmp += c;
		if (c == -32) {//方向鍵
			c = _getch();
			if (c == 80) {//down arrow
				if (nY + 1 < opt.size()) {
					nY++;
					if (opt[nY].length() <= nX) {
						nX = optnX[nY];
					}
				}
			}
			else if (c == 72) {//up arrow
				if (nY > 0) {
					nY--;
					if (opt[nY].length() <= nX) {
						nX = optnX[nY];
					}
				}
			}
			else if (c == 77) {//right arrow
				if (nX < opt[nY].length() - 2) {
					if (opt[nY][nX] & 0x80) {
						nX += 2;
					}
					else {
						nX++;
					}

				}
				else if (nX < opt[nY].length() - 1) {
					nX++;
				}
				else if (nX == opt[nY].length() - 1) {
					if (opt.size() > nY + 1) {
						nY++;
						nX = 0;
					}
				}
			}
			else if (c == 75) {//left arrow
				if (nX > 1) {
					if (opt[nY][nX - 2] & 0x80 && opt[nY][nX - 1] & 0x80) {
						nX -= 2;
					}
					else {
						nX -= 1;
					}
				}
				else if (nX > 0) {
					nX--;
				}
				else if (nX == 0) {
					if (nY > 0) {
						nY--;
						nX = optnX[nY];
					}
				}
			}

		}
		else if (c & 0x80) {
			if (opt[nY].length() + 1 >= textWidth) {
				optnX[nY] = nX;
				nY++;
				nX = 0;
				optnX.insert(optnX.begin() + nY, nX);
				opt.insert(opt.begin() + nY, "");
			}
			tmp += _getch();
			opt[nY].insert(nX, tmp);
			nX += 2;
		}
		else if (c == 13) {//enter
			optnX[nY] = nX;
			nY++;
			nX = 0;
			optnX.insert(optnX.begin() + nY, nX);
			opt.insert(opt.begin() + nY, "");
		}
		else if (c == 8) {//backspace
			if (nX > 1) {//可能有中文
				if (opt[nY][nX - 2] & 0x80) {
					opt[nY].erase(nX - 2, 2);
					nX -= 2;
				}
				else {
					opt[nY].erase(nX - 1);
					nX -= 1;
				}
			}
			else if (nX == 1) {
				opt[nY].erase(nX - 1);
				nX -= 1;
			}
			else if (nX == 0) {
				if (nY > 0) {
					if (opt[nY].length() == 0) {
						opt.erase(opt.begin() + nY);
						optnX.erase(optnX.begin() + nY);
					}
					nY--;
					nX = optnX[nY];
				}
			}
		}
		else if (c == 27) {//esc
			viewer.setNdraw(13, strX, "發送/停留/放棄(Y/S/N)");
			string output = "";
			while (c = _getch()) {
				bool fflag = false;
				switch (c)
				{
				case 'Y':
					for (int i = 0; i < opt.size() - 1; ++i) {
						output += opt[i];
						output += "\n";
					}
					output += opt[opt.size() - 1];
					return output;
				case 'S':
					fflag = true;
					break;
				case 'N':
					return "" + (char)24;
				default:
					continue;
				}
				if (fflag) {
					viewer.clear();
					viewer.setNdraw(0, strX, work);
					for (int i = 0; i < opt.size(); ++i) {
						gotoyx(strY + i, strX);
						cout << opt[i];
					}
					gotoyx(nY, nX);
					break;
				}


			}

		}
		else if (isalnum(c) || c == 32) {
			if (opt[nY].length() >= textWidth) {
				optnX[nY] = nX;
				nY++;
				nX = 0;
				optnX.insert(optnX.begin() + nY, nX);
				opt.insert(opt.begin() + nY, "");
			}
			opt[nY].insert(nX, tmp);
			nX += 1;
		}
		for (int i = 0; i < opt.size(); ++i) {
			string otmp = opt[i] + string(textWidth - opt[i].length(), ' ');
			gotoyx(strY + i, strX);
			cout << otmp;
		}
		gotoyx(strY + nY, strX + nX);
	}
	return "";
}
void BoardManager::profile() {
	viewer.clear();
	viewer.setNdraw(5, 10, "帳號(暱稱): " + users[current_user]->account + "(" + users[current_user]->nickname + ")");
	string cid;
	switch (users[current_user]->identity)
	{
		case 0:
			cid = "Guest";
			break;
		case 1:
			cid = "Member";
			break;
		case 2:
			cid = "Adiministrator";
			break;
	default:
		break;
	}
	viewer.setNdraw(6, 10, "身分組: " + cid);
	viewer.setNdraw(7, 10, "錢: "+ to_string(users[current_user]->money));
	while (true) {
		char chr = _getch();
		if (chr == -32) {//方向鍵
			chr = _getch();
			if (chr == 80) {//down arrow

			}
			else if (chr == 72) {//up arrow

			}
			else if (chr == 77) {//right arrow

			}
			else if (chr == 75) {//left arrow
				state = BoardState::MENU;
				return;
			}
		}
	}
	
}
void BoardManager::post() {
	viewer.clear();
	vector<string> v = boards[current_board].postlist[current_post].toarticle();
	int showlistnum = 15;
	int strX = 5;
	int index = 0;
	while (true) {
		viewer.setNdraw(20, strX, "推(z)噓(x)回覆(c)編輯(e)管理員刪留言(d)");
		for (int i = 0; i + index < v.size() && i < showlistnum; ++i) {
			gotoyx(5 + i, strX);
			viewer.cOneline(5 + i);
			viewer.setNdraw(5 + i, strX, v[i+index]);
		}
		char chr = _getch();
		if (chr == -32) {//方向鍵
			chr = _getch();
			if (chr == 80) {//down arrow
				if (showlistnum + index < v.size() - 1) {
					index++;
				}
			}
			else if (chr == 72) {//up arrow
				if (index > 0) {
					index--;
				}
			}
			else if (chr == 77) {//right arrow

			}
			else if (chr == 75) {//left arrow
				state = BoardState::BOARD;
				return;
			}
			
		}
		else if (chr == 'z') {
			if (users[current_user]->identity == User::Identity::Guest) {
				viewer.cOneline(20);
				viewer.setNdraw(20, strX, "權限不足");
				Sleep(2000);
				viewer.setNdraw(20, strX, "推(z)噓(x)回覆(c)編輯(e)管理員刪留言(d)");
				continue;
			}
			bool exi = false;
			for (vector<string> v : boards[current_board].postlist[current_post].comment) {
				if (v[1] == users[current_user]->account && ((v[1] == "0") || (v[1] == "1"))) {
					exi = true;
					break;
				}
			}
			if (exi) {
				viewer.cOneline(20);
				viewer.setNdraw(20, strX, "已推或噓過");
				Sleep(2000);
				viewer.setNdraw(20, strX, "推(z)噓(x)回覆(c)編輯(e)管理員刪留言(d)");
			}
			string cn = editMode("Re:" + boards[current_board].postlist[current_post].Title);
			if (cn[0] == (char)24) {
				continue;
			}
			else {
				boards[current_board].postlist[current_post].newComment("0", users[current_user]->account, cn);
				v = boards[current_board].postlist[current_post].toarticle();
				viewer.clear();
			}
		}
		else if (chr == 'x') {
			if (users[current_user]->identity == User::Identity::Guest) {
				viewer.cOneline(20);
				viewer.setNdraw(20, strX, "權限不足");
				Sleep(2000);
				viewer.setNdraw(20, strX, "推(z)噓(x)回覆(c)編輯(e)管理員刪留言(d)");
				continue;
			}
			bool exi = false;
			for (vector<string> v : boards[current_board].postlist[current_post].comment) {
				if (v[1] == users[current_user]->account&&((v[1]=="0")||(v[1]=="1"))) {
					exi = true;
					break;
				}
			}
			if (exi) {
				viewer.cOneline(20);
				viewer.setNdraw(20, strX, "已推或噓過");
				Sleep(2000);
				viewer.setNdraw(20, strX, "推(z)噓(x)回覆(c)編輯(e)管理員刪留言(d)");
			}
			string cn = editMode("Re:" + boards[current_board].postlist[current_post].Title);
			if (cn[0] == (char)24) {
				continue;
			}
			else {
				boards[current_board].postlist[current_post].newComment("1", users[current_user]->account, cn);
				v = boards[current_board].postlist[current_post].toarticle();
				viewer.clear();
			}
		}
		else if (chr == 'c') {
			if (users[current_user]->identity == User::Identity::Guest) {
				viewer.cOneline(20);
				viewer.setNdraw(20, strX, "權限不足");
				Sleep(2000);
				viewer.setNdraw(20, strX, "推(z)噓(x)回覆(c)編輯(e)管理員刪留言(d)");
				continue;
			}
			string cn = editMode("Re:" + boards[current_board].postlist[current_post].Title);
			if (cn[0] == (char)24) {
				continue;
			}
			else {
				boards[current_board].postlist[current_post].newComment("2", users[current_user]->account, cn);
				v = boards[current_board].postlist[current_post].toarticle();
				viewer.clear();
			}
		}
		else if (chr == 'd') {
			if (users[current_user]->identity == User::Identity::Adiministrator) {
				viewer.cOneline(20);
				viewer.setNdraw(20, strX, "輸入要刪除的留言編號:");
				int num;
				cin >> num;
				ms.newMail(boards[current_board].postlist[current_post].Title + " 刪留言通知", users[current_user]->account, boards[current_board].postlist[current_post].comment[num][2], editMode("刪留言原因"));
				boards[current_board].postlist[current_post].comment.erase(boards[current_board].postlist[current_post].comment.begin() + num);
				boards[current_board].postlist[current_post].save();
				v = boards[current_board].postlist[current_post].toarticle();
				viewer.cOneline(20);
				viewer.setNdraw(20, strX, "刪除成功");
				Sleep(2000);
				viewer.clear();
			}
			else {
				viewer.cOneline(20);
				viewer.setNdraw(20, strX, "權限不足");
				Sleep(2000);
				viewer.setNdraw(20, strX, "推(z)噓(x)回覆(c)編輯(e)管理員刪留言(d)");
			}
		}
		else if (chr == 'e') {
			if (users[current_user]->account == boards[current_board].postlist[current_post].account) {
				boards[current_board].postlist[current_post].fixcontent(editexist("編輯文章",boards[current_board].postlist[current_post].content));
				v = boards[current_board].postlist[current_post].toarticle();
				refresh();
				viewer.clear();
			}
			else {
				viewer.cOneline(20);
				viewer.setNdraw(20, strX, "非發文者");
				Sleep(2000);
				viewer.setNdraw(20, strX, "推(z)噓(x)回覆(c)編輯(e)管理員刪留言(d)");
			}
		}
	}
}

void BoardManager::board() {
	viewer.clear();
	int showlistnum = 15;
	int strY = 5;
	int index = 0;
	int cursor = 0;
	vector<Post> &posts = boards[current_board].postlist;
	User *nUser = users[current_user];
	if (nUser->identity == User::Identity::Adiministrator) {
		viewer.setNdraw(23, 10, "發文(n)管理員刪文(d)");
	}
	else {
		viewer.setNdraw(23, 10, "發文(n)");
	}
	while (true) {
		for (int i = 0; i + index < posts.size() && i < showlistnum; ++i) {
			gotoyx(strY + i, 20);
			viewer.cOneline(strY + i);
			viewer.setNdraw(strY + i, 20, posts[i + index].Title);
		}
		viewer.setNdraw(strY + cursor, 18, "->");
		char chr = _getch();
		if (chr == -32) {//方向鍵
			chr = _getch();
			if (chr == 80) {//down arrow
				if (index + cursor < posts.size() - 1) {
					if (cursor < showlistnum - 1) {
						cursor += 1;
					}
					else if (cursor == showlistnum - 1) {
						index += 1;
					}

				}
			}
			else if (chr == 72) {//up arrow
				if (cursor > 0) {
					cursor -= 1;
				}
				else if (cursor == 0 && index > 0) {
					index -= 1;
				}
			}
			else if (chr == 77) {//right arrow
				 current_post = cursor + index;
				 state = BoardState::POST;
				return;
			}
			else if (chr == 75) {//left arrow
				state = BoardState::SELECT_BOARD;
				return;
			}
		}
		else if (chr == 'n') {
			viewer.cOneline(23);
			if (users[current_user]->identity == User::Identity::Guest) {
				viewer.setNdraw(23, 10, "權限不足");
				Sleep(2000);
				viewer.cOneline(23);
				viewer.setNdraw(23, 10, "發文(n)");
				continue;
			}
			else {
				viewer.setNdraw(23, 10, "請輸入標題:");
			}
			string title;
			cin >> title;
			boards[current_board].newPost(title, users[current_user]->account, users[current_user]->nickname, editMode("你正發文在" + boards[current_board].name));
			viewer.clear();
			if (nUser->identity == User::Identity::Adiministrator) {
				viewer.setNdraw(23, 10, "發文(n)管理員刪文(d)");
			}
			else {
				viewer.setNdraw(23, 10, "發文(n)");
			}
		}
		else if (chr == 'd') {
			if (nUser->identity == User::Identity::Adiministrator) {
				viewer.cOneline(23);
				viewer.setNdraw(23, 10, "確認刪文(y/n):");
				char tc;
				while (tc = _getch()) {
					if (tc == 'y') {
						ms.newMail(boards[current_board].postlist[index + cursor].Title + " 刪文通知",
							nUser->account, boards[current_board].postlist[index + cursor].account, editMode("刪文原因"));
						boards[current_board].deletePost(index + cursor);
						viewer.cOneline(23);
						viewer.setNdraw(23, 10, "刪文成功");
						refresh();
						Sleep(2000);
						viewer.clear();
						viewer.setNdraw(23, 10, "發文(n)管理員刪文(d)");
						break;
					}
					else if (tc == 'n') {
						viewer.cOneline(23);
						viewer.setNdraw(23, 10, "取消刪文");
						Sleep(2000);
						viewer.cOneline(23);
						viewer.setNdraw(23, 10, "發文(n)管理員刪文(d)");
					}
					
				}
				
			}
			else {
				continue;
			}
			
		}
	}
}


void BoardManager::selectboard() {
	viewer.clear();
	int showlistnum = 15;
	int index = 0;
	int cursor = 0;
	while (true) {
		viewer.setNdraw(23, 10, "創板(n)刪板(d)");
		for (int i = 0; i + index < boards.size() && i < showlistnum; ++i) {
			int hot = boards[i + index].postlist.size();
			for (Post p : boards[i + index].postlist) {
				hot += p.comment.size();
			}
			string ht = to_string(hot);
			ht = string(4 - ht.length(), ' ') + ht;
			gotoyx(10 + i, 20);
			viewer.cOneline(10 + i);
			viewer.setNdraw(10 + i, 20, boards[i + index].name + string(12- boards[i + index].name.length(),' ')+"人氣 " +ht );
		}
		viewer.setNdraw(10 + cursor, 18, "->");
		char chr = _getch();
		if (chr == -32) {//方向鍵
			chr = _getch();
			if (chr == 80) {//down arrow
				if (index + cursor < boards.size() - 1) {
					if (cursor < showlistnum - 1) {
						cursor += 1;
					}
					else if (cursor == showlistnum - 1) {
						index += 1;
					}

				}
			}
			else if (chr == 72) {//up arrow
				if (cursor > 0) {
					cursor -= 1;
				}
				else if (cursor == 0 && index > 0) {
					index -= 1;
				}
			}
			else if (chr == 77) {//right arrow
				current_board = cursor + index;
				state = BoardState::BOARD;
				return;
			}
			else if (chr == 75) {//left arrow
				state = BoardState::MENU;
				return;
			}
		}
		else if (chr == 'n') {
			if (users[current_user]->identity == User::Identity::Adiministrator) {
				viewer.cOneline(23);
				viewer.setNdraw(23, 10, "請輸入板名:");
				string name = viewer.getInput(23,21,15);
				Board b(name);
				viewer.cOneline(23);
				viewer.setNdraw(23, 10, "創板(n)刪板(d)");
				refresh();
			}
			else {
				viewer.cOneline(23);
				viewer.setNdraw(23, 10, "權限不足");
				Sleep(2000);
				viewer.cOneline(23);
				viewer.setNdraw(23, 10, "創板(n)刪板(d)");
			}
		}
		else if (chr == 'd') {
			if (users[current_user]->identity == User::Identity::Adiministrator) {
				viewer.cOneline(23);
				viewer.setNdraw(23, 10, "確認刪除(y/n):");
				char tc;
				while (tc = _getch()) {
					if (tc == 'y') {
						deleteboard(index + cursor);
						viewer.cOneline(23);
						viewer.setNdraw(23, 10, "刪除成功");
						Sleep(2000);
						viewer.clear();
						break;
					}
					else if(tc == 'n') {
						viewer.cOneline(23);
						viewer.setNdraw(23, 10, "取消刪除");
						Sleep(2000);
						break;
					}
				}
				viewer.cOneline(23);
				viewer.setNdraw(23, 10, "創板(n)刪板(d)");
				refresh();
			}
			else {
				viewer.cOneline(23);
				viewer.setNdraw(23, 10, "權限不足");
				Sleep(2000);
				viewer.cOneline(23);
				viewer.setNdraw(23, 10, "創板(n)刪板(d)");
			}
		}
	}

}

void BoardManager::menu() {
	int showlistnum = 15;
	viewer.clear();
	vector<string> MENU;
	MENU.push_back("無分組討論版");
	MENU.push_back("個人資訊");
	MENU.push_back("MAIL");
	MENU.push_back("發文紀錄");
	MENU.push_back("小遊戲");
	MENU.push_back("登出");
	
	int index = 0;
	int cursor = 0;
	while (true) {
		for (int i = 0; i+index < MENU.size() && i < showlistnum; ++i) {
			gotoyx(10+i, 20);
			viewer.cOneline(10 + i);
			viewer.setNdraw(10+i, 20, MENU[i+index]);
		}
		viewer.setNdraw(10 + cursor, 18, "->");
		char chr = _getch();
		if (chr == -32) {//方向鍵
			chr = _getch();
			if (chr == 80) {//down arrow
				if (index+cursor < MENU.size() - 1) {
					if (cursor < showlistnum-1) {
						cursor += 1;
					}
					else if (cursor == showlistnum - 1) {
						index += 1;
					}

				}
			}
			else if (chr == 72) {//up arrow
				if (cursor > 0) {
					cursor -= 1;
				}
				else if (cursor == 0 && index > 0) {
					index -= 1;
				}
			}
			else if (chr == 77) {//right arrow
				if (users[current_user]->identity == User::Identity::Guest) {
					if ((index + cursor == 0) || (index + cursor == 5)) {
						break;
					}
					else {
						viewer.setNdraw(23, 5, "訪客無法使用");
						Sleep(1500);
						viewer.cOneline(23);
					}
				}
				else {
					break;
				}
				
				
			}
		}
		
	}
	switch (cursor + index) {
		case 0:
			state = BoardState::SELECT_BOARD;
			return;
		case 1:
			state = BoardState::PROFILE;
			return;
		case 2:
			state = BoardState::MAIL;
			return;
		case 3:
			state = BoardState::POSTHISTORY;
			return;
		case 4:
			state = BoardState::GAME;
			return;
		case 5:
			state = BoardState::LOGIN;
			return;
	}
	
}
void BoardManager::showMail(Mail m) {
	viewer.clear();
	viewer.setNdraw(5, 5, "寄件者   "+m.sender);
	viewer.setNdraw(6, 5, "收件者   " + m.reciver);
	viewer.setNdraw(7, 5, "標題     " + m.title);
	viewer.setNdraw(8, 5, "發信時間 " + m.time);
	viewer.setNdraw(23, 5, "退出(esc)");


	int sY = 10;
	gotoyx(sY, 5);
	for (int i = 0; i < m.content.length(); ++i) {
		if (m.content[i] == '\n') {
			gotoyx(++sY, 5);
			continue;
		}
		string tmp = "";
		tmp += m.content[i];
		if (m.content[i] & 0x80) {
			tmp += m.content[++i];
			cout << tmp;
		}
		else {
			cout << tmp;
		}
	}
	char chr;
	while (chr = _getch()) {
		if (chr == 27) {//esc
			viewer.clear();
			return;
		}
	}
}

void BoardManager::mail() {
	User* nU = users[current_user];
	viewer.clear();
	
	vector<Mail> mo, mi;
	for (Mail m : ms.mails) {
		if (m.sender == nU->account) {
			mo.push_back(m);
		}
		else if (m.reciver == nU->account) {
			mi.push_back(m);
		}
	}
	int showlistnum = 15;
	int strY = 6;
	int index = 0;
	int cursor = 0;
	vector<Mail> cm;
	int currentMode = 0;//0看收件 1看寄出
	while (true) {
		viewer.setNdraw(23, 5, "寫信(n)收件夾/寄信備份(tab)");
		if (currentMode == 0) {
			cm = mi;
			viewer.setNdraw(5, 5, "寄件者");
			viewer.setNdraw(5, 15, "標題");
			viewer.setNdraw(5, 45, "時間");
		}
		else if (currentMode == 1) {
			cm = mo;
			viewer.setNdraw(5, 5, "收件者");
			viewer.setNdraw(5, 15, "標題");
			viewer.setNdraw(5, 45, "時間");
		}
		for (int i = 0; i + index < cm.size() && i < showlistnum; ++i) {
			gotoyx(strY + i, 20);
			viewer.cOneline(strY + i);
			viewer.setNdraw(strY + i, 5, cm[i].sender);
			viewer.setNdraw(strY + i, 15, cm[i].title);
			viewer.setNdraw(strY + i, 45, cm[i].time);
		}
		viewer.setNdraw(strY + cursor, 3, "->");
		char chr = _getch();
		if (chr == -32) {//方向鍵
			chr = _getch();
			if (chr == 80) {//down arrow
				if (cm.size() == 0) {
					continue;
				}
				if (index + cursor < cm.size() - 1) {
					if (cursor < showlistnum - 1) {
						cursor += 1;
					}
					else if (cursor == showlistnum - 1) {
						index += 1;
					}

				}
			}
			else if (chr == 72) {//up arrow
				if (cm.size() == 0) {
					continue;
				}
				if (cursor > 0) {
					cursor -= 1;
				}
				else if (cursor == 0 && index > 0) {
					index -= 1;
				}
			}
			else if (chr == 77) {//right arrow
				showMail(cm[cursor + index]);
			}
			else if (chr == 75) {//left arrow
				state = BoardState::MENU;
				return;
			}
		}
		else if (chr == 'n') {
			viewer.cOneline(23);
			viewer.setNdraw(23, 5, "輸入收信對象:");
			string tar = viewer.getInput(23, 18, 10);
			bool fflag = false;
			for (User* u : users) {
				if (u->account == tar) {
					fflag = true;
					break;
				}
			}
			if (fflag) {
				viewer.cOneline(23);
				viewer.setNdraw(23, 5, "輸入標題:");
				string ti = viewer.getInput(23, 14, 10);
				string cn = editMode("你正寫信給 " + tar);
				viewer.clear();
				if (cn[0] == (char)24) {
					viewer.cOneline(23);
					viewer.setNdraw(23, 5, "取消發送");
					Sleep(2000);
					continue;
				}
				ms.newMail(ti, nU->account, tar, cn);
				viewer.cOneline(23);
				viewer.setNdraw(23, 5, "發送成功");
				Sleep(2000);
				viewer.cOneline(23);
			}
			else {
				viewer.cOneline(23);
				viewer.setNdraw(23, 5, "查無對象");
				Sleep(2000);
				viewer.cOneline(23);
			}
		}
		else if (chr == 9) {
			if (currentMode == 0) {
				currentMode = 1;
			}
			else if (currentMode == 1) {
				currentMode = 0;
			}
		}
		
	}
}

void BoardManager::signup() {
	viewer.clear();
	viewer.setWord(5, 20, "按下ESC以放棄註冊");
	
	
	//帳號
	viewer.setWord(8, 20, "帳號 ");
	
	for (int i = 0; i < 10; ++i)
		viewer.color[8][25 + i] = 119;
	viewer.setWord(8, 25, string(10, ' '));

	viewer.draw();

	SetColor(112);
	string account;
	while (true) {
		bool exi = false;
		viewer.cOneline(8);
		 viewer.setNdraw(8, 20, "帳號 ");
		 account = viewer.getInput(8, 25, 9);
		 for (User* u : users) {
			 if (u->account == account) {
				 exi = true;
				 break;
			 }
		 }
		 if (!exi) {
			 break;
		 }
	}
	
	SetColor(7);

	if (account.empty()) {
		return;
	}
	viewer.setWord(8, 25, account);
	for (int i = 0; i < 9; ++i)
		viewer.color[8][25 + i] = 112;

	viewer.draw();
	//帳號

	//密碼
	viewer.setWord(13, 20, "密碼 ");

	for (int i = 0; i < 10; ++i)
		viewer.color[13][25 + i] = 119;
	viewer.setWord(13, 25, string(10, ' '));

	viewer.draw();

	SetColor(112);
	string password = viewer.getInput(13, 25, 9);
	SetColor(7);

	if (password.empty()) {
		return;
	}
	viewer.setWord(13, 25, password);
	for (int i = 0; i < 9; ++i)
		viewer.color[13][25 + i] = 112;

	viewer.draw();
	//密碼

	//暱稱
	viewer.setWord(18, 20, "暱稱 ");

	for (int i = 0; i < 10; ++i)
		viewer.color[18][25 + i] = 119;
	viewer.setWord(18, 25, string(10, ' '));

	viewer.draw();

	SetColor(112);
	string nickname = viewer.getInput(18, 25, 9);
	SetColor(7);

	if (nickname.empty()) {
		return;
	}
	viewer.setWord(18, 25, nickname);
	for (int i = 0; i < 9; ++i)
		viewer.color[18][25 + i] = 112;

	viewer.draw();
	//暱稱

	User(account, password, nickname);

	refresh();
}
void BoardManager::login() {
	string acin;
	int strY = 23;
	while (true) {
		viewer.clear();
		
		///設定畫面
		viewer.setWord(strY, 20, "請輸入代號，或以 guest 參觀，或以 new 註冊:");
		for (int i = 0; i < 10; ++i)
			viewer.color[strY][63 + i] = 119;
		viewer.setWord(strY, 63, string(10, ' '));
		///設定畫面結束
		viewer.draw();

		//取得輸入
		SetColor(112);
		acin = viewer.getInput(strY,63,9);
		SetColor(7);
		//取得輸入結束

		//保留帳號顯示
		viewer.setWord(strY, 63, acin);
		for (int i = 0; i < 9; ++i)
			viewer.color[strY][63 + i] = 112;
		//

		if (acin == "guest") {
			current_user = 0;//guest的使用者順位為0
			state = BoardState::MENU;
			return;
		}
		if (acin == "new") {
			signup();
			continue;
		}

		viewer.setWord(strY+1, 20, "請輸入您的密碼:");
		viewer.draw();
		
		string pwin;
		SetColor(0);
		pwin = viewer.getInput(strY+1,35,9);
		SetColor(7);

		for (int i = 0; i < users.size(); ++i) {
			
			if (users[i]->account == acin) {
				if (pwin == users[i]->password) {
					current_user = i;
					users[i]->LastLogTime = getTimeString();
					//進入下一步
					state = BoardState::MENU;
					return;
				}
				else {
					break;
				}
			}
		}
		viewer.cOneline(strY+1);
		viewer.setWord(strY+1, 20, "密碼不對或無此帳號。請檢查大小寫及有無輸入錯誤。");
		viewer.draw();
		Sleep(2000);
	}
}

void BoardManager::refresh() {
	users.clear();
	ifstream ifs;
	ifs.open(".\\User\\userlist.txt");
	string tmp;
	while (ifs >> tmp) {
		ifstream userifs;
		string file = ".\\User\\" + tmp + ".txt";
		userifs.open(".\\User\\" + tmp + ".txt");
		int identity;
		userifs >> identity;
		if (identity == 1) {//member
			users.push_back(new Member(tmp));
		}
		else if (identity == 2) {
			users.push_back(new Adiministrator(tmp));
		}
		else if (identity == 0) {
			users.push_back(new Guest(tmp));
		}
		userifs.close();
	}
	ifs.close();

	boards.clear();

	ifs.open(".\\Board\\boardlist.txt");
		while (ifs >> tmp) {
			if (tmp[0] == '!')
				continue;
			ifstream boardifs;
			string file = ".\\Board\\" + tmp + ".txt";
			boards.push_back(Board(tmp));
			boardifs.close();
		}
}
