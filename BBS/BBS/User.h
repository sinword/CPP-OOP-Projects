#include <string>
#include <vector>
using namespace std;
class User {
public:
	enum Identity
	{
		Guest,
		Member,
		Adiministrator
	};
	string account;
	string nickname;
	string password;
	string LastLogIP;
	string LastLogTime;
	Identity identity;
	int money;
	vector<string> postRecord;
	User();
	User(string file);//讀入使用者
	User(string account, string password, string nickname);//新建使用者
	void save();
};
class Guest :public User {
public:
	Guest(string file) :User(file) {};
};
class Member :public User {
public:
	Member(string file) :User(file) {};

};
class Adiministrator :public User {
public:
	Adiministrator(string file) :User(file) {};

};
string getTimeString();