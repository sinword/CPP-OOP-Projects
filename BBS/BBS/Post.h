#include <fstream>
#include <vector>
#include <string>
#include <ctime>
using namespace std;
class Post {		//概念 每個動作都寫檔 但程式中使用物件
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
	vector<vector<string>> comment;//留言包含推噓作者內容        
	Post(string file,string board);//讀入 使用完整路徑
	Post(string title, string account,string nickname, string content, string board);//新建
	void fixcontent(string);//修改內容
	void hidePost();
	vector<string> toarticle();
};

