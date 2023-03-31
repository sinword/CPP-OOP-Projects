#include "User.h"
#include <ctime>
#include <Winsock2.h>
#include <iphlpapi.h>
#include "addtionalTool.h"
#include <fstream>
#pragma comment(lib, "IPHLPAPI.lib")
using namespace std;
User::User() {
	this->identity = Identity::Guest;
}

User::User(string file) {
	ifstream ifs;
	ifs.open(".\\User\\" + file + ".txt");
	int tmp;
	ifs >> tmp;
	if (tmp == 0) {
		identity = Identity::Guest;
		account = "guest";
		nickname = "guest";
		password = "";
		LastLogTime = "";
		money = 0;
		
	}
	else {
		if (tmp == 1) {
			this->identity = Identity::Member;
		}
		else if (tmp == 2) {
			this->identity = Identity::Adiministrator;
		}
		ifs >> this->account;
		ifs >> this->nickname;
		ifs >> this->password;
		ifs >> this->LastLogTime;
		ifs >> this->money;
		while (!ifs.eof()) {
			string path;
			ifs >> path;
			postRecord.push_back(path);
		}
	}
	


}
void User::save() {
	ofstream ofs;
	ofs.open(".\\User\\" + account + ".txt");
	ofs << identity << "\n";
	ofs << account << "\n";
	ofs << nickname << "\n";
	ofs << password << "\n";
	ofs << LastLogTime << "\n";
	ofs << money << "\n";
	for (string s : postRecord) {
		ofs << s << "\n";
	}
	ofs.close();
}

User::User(string account, string password, string nickname) {
	this->identity = Identity::Member;
	this->account = account;
	this->password = password;
	this->nickname = nickname;
	this->LastLogTime = getTimeString();
	this->money = 0;
	ofstream ofs;
	ofs.open(".\\User\\userlist.txt", std::ofstream::out | std::ofstream::app);
	ofs << account << "\n";
	ofs.close();
	ofs.open(".\\User\\" + account + ".txt");
	/*string account;
	string nickname;
	string password;
	string LastLogIP;
	string LastLogTime;
	Identity identity;*/
	ofs << identity << "\n";
	ofs << account << "\n";
	ofs << nickname << "\n";
	ofs << password << "\n";
	ofs << LastLogTime << "\n";
	ofs << money << "\n";
}
string getIP() {
	unsigned long outBufLen = 0;
	GetAdaptersAddresses(AF_UNSPEC, 0, NULL, NULL, &outBufLen);
	PIP_ADAPTER_ADDRESSES pAddresses = (IP_ADAPTER_ADDRESSES*)malloc(outBufLen);

	GetAdaptersAddresses(AF_UNSPEC, GAA_FLAG_SKIP_ANYCAST, NULL, pAddresses, &outBufLen);
	return "";
}
