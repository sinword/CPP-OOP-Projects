#include "addtionalTool.h"
#include <fstream>
#include <set>
using namespace std;
#pragma warning(disable : 4996) 
string getCleanTimeString() {
	string date;
	time_t now = time(0);
	tm* ltm = localtime(&now);
	int year = 1900 + ltm->tm_year;
	date += to_string(year);
	date += to_string(ltm->tm_mon);
	date += to_string(ltm->tm_mday);
	date += to_string(ltm->tm_hour)+to_string(ltm->tm_min) + to_string(ltm->tm_sec);
	return date;
}

string getTimeString() {
	string date;
	time_t now = time(0);
	tm* ltm = localtime(&now);
	int year = 1900 + ltm->tm_year;
	date += to_string(year) + "¦~";
	date += to_string(ltm->tm_mon) + "¤ë";
	date += to_string(ltm->tm_mday) + "¤é";
	date += to_string(ltm->tm_hour) + ":" + to_string(ltm->tm_min) + ":" + to_string(ltm->tm_sec);
	return date;
}
string generateID() {
	srand(time(NULL));
	ifstream ifs;
	ifs.open("IDlist.txt");
	set<string> idSet;
	string tempID;
	while (ifs >> tempID) {
		idSet.insert(tempID);
	}
	ifs.close();
	while (true) {
		string s;
		for (int i = 0; i < 16; ++i) {
			if (i != 0 && i % 4 == 0) {
				s += "-";
			}
			int tmp = rand() % 62;
			if (tmp < 10) {
				s += ('0' + tmp);
			}
			else if (tmp >= 10 && tmp < 36) {
				tmp -= 10;
				char chr = tmp + 'A';
				s += chr;
			}
			else {
				tmp -= 36;
				char chr = tmp + 'a';
				s += chr;
			}
		}
		if (idSet.find(s) == idSet.end()) {
			ofstream ofs;
			ofs.open("IDlist.txt");
			idSet.insert(s);
			for (string ts : idSet) {
				ofs << ts << "\n";
			}
			ofs.close();
			return s;
		}
	}
	
}