#include "mail.h"
Mail::Mail(string id,string ti,string sr, string rr, string t, string cn) {
	ID = id;
	title = ti;
	sender = sr;
	reciver = rr;
	time = t;
	content = cn;
}
void MailSystem::update() {
	ifstream ifs;
	ifs.open(".\\Mail\\maillist.txt");
	string tmp;
	mails.clear();
	while (ifs >> tmp) {
		ifstream mfs;
		mfs.open(".\\Mail\\" + tmp + ".txt");
		string ID, title,sender, reciver, time, content;
		string ul;
		getline(mfs, ID);
		getline(mfs, title);
		getline(mfs, sender);
		getline(mfs, reciver);
		getline(mfs, time);
		getline(mfs, content,(char)3);
		getline(mfs, ul);
		mails.push_back(Mail(ID, title, sender, reciver, time, content));
	}
}
void MailSystem::newMail(string title,string sender, string reciver, string content) {
	string time = getTimeString();
	string ID = generateID();
	mails.push_back(Mail(ID, title, sender, reciver, time, content));
	ofstream ofs;
	ofs.open(".\\Mail\\maillist.txt", std::ofstream::out | std::ofstream::app);
	ofs << ID << "\n";
	ofs.close();
	ofs.open(".\\Mail\\" + ID + ".txt");
	ofs << ID << "\n";
	ofs << title << "\n";
	ofs << sender << "\n";
	ofs << reciver << "\n";
	ofs << time << "\n";
	ofs << content << (char)3 << "\n";
	ofs.close();
	
}
MailSystem::MailSystem() {
	update();
}