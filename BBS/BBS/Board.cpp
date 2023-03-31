#include "Board.h"
#include <direct.h>
Board::Board(string file) {//file為版名
	this->name = file;
	string path = ".\\Board\\boardlist.txt";
	ifstream ifs;
	ifs.open(path);
	vector<string> v;
	string tmp;
	bool exist = false;
	while (ifs >> tmp) {
		v.push_back(tmp);
		if (tmp == file) {
			exist = true;
		}
	}
	ifs.close();

	if (!exist) {
		v.push_back(file);
		ofstream ofs;
		ofs.open(path);
		ofs.clear();
		for (string s : v) {
			ofs << s << "\n";
		}
		ofs.close();
		string dirPath = ".\\Board\\" + file;
		_mkdir(dirPath.c_str());
		ofs.open(".\\Board\\" + file + "\\postlist.txt");
		ofs << " ";
		ofs.close();
	}

	
	ifs.open(".\\Board\\" + file + "\\postlist.txt");
	string post;
	while (ifs >> post) {
		if (post[0] == '!') {//排除掉隱藏文章
			continue;
		}
		this->postlist.push_back(Post(post,file));
	}
	
	
}
void Board::deletePost(int index) {
	postlist[index].ID = "!" + postlist[index].ID;
	ofstream ofs;
	ofs.open(".\\Board\\" + name + "\\postlist.txt");
	ofs.clear();
	for (Post p : postlist) {
		ofs << p.ID << "\n";
	}
	ofs.close();
}
void Board::newPost(string title, string account, string nickname, string content) {
	postlist.push_back(Post(title, account, nickname, content, name));
}
