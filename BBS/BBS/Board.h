#include <vector>
#include "Post.h"
class Board {
public:
	string name;
	vector<Post> postlist;
	Board(string);//輸入版名 存在就讀入 不存在就新增
	void deletePost(int);
	void newPost(string title, string account, string nickname, string content);
};