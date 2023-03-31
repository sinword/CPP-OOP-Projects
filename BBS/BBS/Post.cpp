#include "Post.h"
#include "addtionalTool.h"
#include <iostream>
#include <sstream>

Post::Post(string title, string account,string nickname, string content,string board) {
	if (content == "" + (char)24) {
		return;
	}
	this->ID = generateID();
	this->Title = title;
	this->content = content;
	//目前content採取普通輸入
	this->date = getTimeString();
	this->account = account;
	this->nickname = nickname;
	this->board = board;
	string path = ".\\Board\\" + board + "\\";
	ofstream ofs;
	ofs.open(path + "postlist.txt", std::ofstream::out | std::ofstream::app);
	ofs << ID << "\n";
	ofs.close();
	ofs.open(path + ID + ".txt");
	ofs << ID << "\n";
	ofs << board << "\n";
	ofs << Title << "\n";
	ofs << account << "\n";
	ofs << nickname << "\n";
	ofs << date << "\n";
	ofs << content << (char)3;
	ofs.close();
	ofs.open(".\\User\\" + account + ".txt", std::ofstream::out | std::ofstream::app);
	ofs << this->ID << "\n";
	ofs.close();
}
Post::Post(string file, string board) {
	this->board = board;
	ifstream ifs;
	ifs.open(".\\Board\\"+board+"\\"+file+".txt");
	getline(ifs, this->ID);
	getline(ifs, this->board);
	getline(ifs, this->Title);
	getline(ifs, this->account);
	getline(ifs, this->nickname );
	getline(ifs, this->date );
	getline(ifs, this->content, (char)3);
	//getline(ifs, this->content, '!');
	while (!isalnum(ifs.peek())&&!ifs.eof()) {
		ifs.get();
	}
	string sign, ac, cn, time;//帳號 內容
	while (getline(ifs, sign) &&
		getline(ifs, ac) &&
		getline(ifs, cn,(char)3) &&
		getline(ifs,time)&&
		getline(ifs, time)) {

		/*getline(ifs, sign);
		getline(ifs,ac);
		getline(ifs,cn);
		getline(ifs, time);*/
		vector<string>tmp;
		tmp.resize(4);
		tmp[0] = sign;
		tmp[1] = ac;
		tmp[2] = cn;
		tmp[3] = time;
		this->comment.push_back(tmp);
	}
}
void Post::newComment(string sign, string ac, string cn) {
	string time = getTimeString();
	vector<string> v;
	v.resize(4);
	v[0] = sign;
	v[1] = ac;
	v[2] = cn+(char)3;
	v[3] = time;
	comment.push_back(v);
	save();
}

void Post::save() {
	ofstream ofs;
	ofs.open(".\\Board\\" + board + "\\" + ID + ".txt");
	ofs.clear();
	ofs << ID << "\n";
	ofs << board << "\n";
	ofs << Title << "\n";
	ofs << account << "\n";
	ofs << nickname << "\n";
	ofs << date << "\n";
	ofs << content << (char)3 << "\n";
	for (vector<string> v: comment) {
		ofs << v[0] << "\n";
		ofs << v[1] << "\n";
		ofs << v[2] << (char)3 << "\n";
		ofs << v[3] << "\n";
	}
	ofs.close();
}

vector<string> Post::toarticle() {
	vector<string> v;
	v.push_back(Title);
	v.push_back("作者" + account + "(" + nickname + ")");
	v.push_back(date);
	stringstream ss(content);
	string tmp;
	while(getline(ss,tmp)){
		v.push_back(tmp);
	}
	int time = -1;
	for (vector<string>c : comment) {
		time++;
		string opt;
		opt += to_string(time) + " ";
		switch (stoi(c[0]))
		{
			case 0:
				opt += "推 ";
				break;
			case 1:
				opt += "噓 ";
				break;
			case 2:
				opt += "→ ";
				break;
			default:
			break;
		}
		opt += c[1] + "  :  ";
		string tem = opt;
		int times = 0;
		for (char chr : c[2]) {
			if (times > 0) {
				tem.replace(0,2, "→");
			}
			if (chr == '\n') {
				opt += string(50 - opt.length(), ' ');
				opt += c[3];
				v.push_back(opt);
				opt = tem;
				times++;
				continue;
			}
			opt += chr;
		}
		opt += string(50 - opt.length(), ' ');
		opt += c[3];
		v.push_back(opt);
	}
	return v;
}
void Post::fixcontent(string content) {
	this->content = content;
	string path = ".\\Board\\" + board + "\\";
	ofstream ofs;
	ofs.open(path + ID + ".txt");
	ofs.clear();
	ofs << ID << "\n";
	ofs << board << "\n";
	ofs << Title << "\n";
	ofs << account << "\n";
	ofs << nickname << "\n";
	ofs << date << "\n";
	ofs << content << (char)3;//這是改過的
	for (vector<string> v : comment) {
		ofs << v[0] << "\n";
		ofs << v[1] << "\n";
		ofs << v[2] << (char)3 << "\n";
		ofs << v[3] << "\n";
	}
	ofs.close();
}
void Post::hidePost() {//在list前加上!以表示不供使用
	vector<string> v;
	string path = ".\\Board\\" + board + "\\";
	ifstream ifs;
	ifs.open(path + "postlist.txt");
	string tmp;
	while (ifs >> tmp) {
		if (tmp == ID) {
			tmp = "!" + tmp;
		}
		v.push_back(tmp);
	}
	ifs.close();

	ofstream ofs;
	ofs.open(path + "postlist.txt");
	ofs.clear();
	for (string s : v) {
		ofs << s << "\n";
	}
	ofs.close();
}






