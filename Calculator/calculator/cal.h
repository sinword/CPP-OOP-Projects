#include "NumberObject.h"
#include <vector>
#include <map>
class cal {
public:
	vector<string> input;
	string result;
	cal(string);
	cal(string,bool);
	static map<string, NumberObject> dic;
};