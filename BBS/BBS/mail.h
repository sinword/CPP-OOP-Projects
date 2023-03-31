#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include "addtionalTool.h"
using namespace std;
class Mail {
public:
	string ID, title,sender, reciver, time, content;
	Mail(string,string,string,string,string,string);//¯Âª«¥ó
};
class MailSystem {
public:
	vector<Mail> mails;
	void newMail(string,string, string, string);
	void update();
	MailSystem();
};
