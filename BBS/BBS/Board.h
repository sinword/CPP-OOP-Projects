#include <vector>
#include "Post.h"
class Board {
public:
	string name;
	vector<Post> postlist;
	Board(string);//��J���W �s�b�NŪ�J ���s�b�N�s�W
	void deletePost(int);
	void newPost(string title, string account, string nickname, string content);
};