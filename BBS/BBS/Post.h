#include <fstream>
#include <vector>
#include <string>
#include <ctime>
using namespace std;
class Post {		//���� �C�Ӱʧ@���g�� ���{�����ϥΪ���
public:
	string ID;
	string Title;
	string account;
	string nickname;
	string date;
	string content;
	string ip;
	string board;
	void save();
	void newComment(string,string,string);
	vector<vector<string>> comment;//�d���]�t���N�@�̤��e        
	Post(string file,string board);//Ū�J �ϥΧ�����|
	Post(string title, string account,string nickname, string content, string board);//�s��
	void fixcontent(string);//�ק鷺�e
	void hidePost();
	vector<string> toarticle();
};

