#include "cal.h"
#include <stack>
#include <sstream>
map<string, NumberObject> cal::dic;
bool isnum(string s) {
	for (char c : s) {
		if (isdigit(c))
			return true;
	}
	return false;
}
int pri(string s) {
	if (s == "(") {
		return 0;
	}
	else if (s == "!") {
		return 9;
	}
	else if (s == "^") {
		return 8;
	}
	else if (s == "p" || s == "n") {
		return 7;
	}
	else if (s == "*" || s == "/") {
		return 6;
	}
	else if (s == "+" || s == "-") {
		return 5;
	}
}

string ssqrt(string input) {
	stringstream ss(input);
	string fore, back;
	getline(ss, fore, '.');
	getline(ss, back);
	int dot = input.find(".");
	if (dot != input.npos) {
		dot = dot / 2;
	}
	else {
		dot = input.length() / 2;
	}
	bool first = true;
	string output = "";
	int i = 0;
	string np = "";
	string qu = "";
	while (output.length() - dot - 2 != 100) {
		if (output.length() == dot + 1)
			output += ".";
		if (input.length() - i < 2) {
			input += "00";
		}
		if (first) {
			if (fore.length() % 2 == 1) {
				np += string(input.begin() + i, input.begin() + i + 1);
			}
			else {
				np += string(input.begin() + i, input.begin() + i + 2);
			}
		}
		else {
			np += string(input.begin() + i, input.begin() + i + 2);
		}
		for (int j = 1; j <= 10; ++j) {
			string tqu = qu + to_string(j);
			string tmppow = mul(tqu, to_string(j));
			if (compare(tmppow, np) == 1) {
				tqu = qu + to_string(j - 1);
				tmppow = mul(tqu, to_string(j - 1));
				np = sub(np, tmppow);
				qu = mul(qu, "10");
				qu = add(qu, add(to_string(j - 1), to_string(j - 1)));

				//qu = basicint(qu, "10").mul();
				output += to_string(j - 1);
				break;
			}
		}
		if (first) {
			if (fore.length() % 2 == 1) {
				i += 1;
			}
			else {
				i += 2;
			}
			first = false;
		}
		else {
			i += 2;
		}
	}
	return output;
}
string sssqrt(string ipt) {
	string f, b;
	stringstream ss(ipt);
	getline(ss, f, '.');
	getline(ss, b);
	int dot = f.length() / 2 + f.length() % 2;
	bool sig = false;
	if (f.length() % 2 == 1) {
		sig = true;
	}
	string s = f + b;
	string opt = "";
	string down = "";
	string right = "";
	string lastright = "";
	int len;
	int count = 0;
	while (true) {
		count++;
		lastright = right;
		/*cout << count << "\n";
		cout << "opt" << opt << "\n";
		cout << "right" << right << "\n";
		cout << "down" << down << "\n";*/

		
		len = opt.length() - dot - 1;
		if (len > 101) {
			break;
		}
		s += "00";
		if (opt.length() == dot) {
			opt += ".";
		}
		if (sig) {
			down += s[0];
			s.erase(s.begin());
			for (int i = 1; i < 10; ++i) {
				string tmp = right + to_string(i);
				string tt = mul(tmp, to_string(i));
				int res = compare(tt, down);
				if (res == 1) {
					tmp = right + to_string(i - 1);
					down = sub(down, mul(tmp, to_string(i - 1)));
					right += to_string(i - 1);
					right = add(right, to_string(i - 1));
					opt += to_string(i - 1);
					sig = false;
					break;
				}
				else if (res == 3) {
					down = sub(down, mul(tmp, to_string(i)));
					right += to_string(i);
					right = add(right, to_string(i));
					opt += to_string(i);
					sig = false;
					break;
				}
			}
		}
		else {
			down += string(s.begin(), s.begin() + 2);
			s.erase(s.begin());
			s.erase(s.begin());
			for (int i = 1;i<10; ++i) {
				string tmp = right + to_string(i);
				string tt = mul(tmp, to_string(i));
				int res = compare(tt, down);
				if (res == 1) {
					tmp = right + to_string(i - 1);
					down = sub(down, mul(tmp, to_string(i - 1)));
					right += to_string(i - 1);
					right = add(right, to_string(i - 1));
					opt += to_string(i - 1);
					break;
				}
				else if (res == 3) {
					down = sub(down, mul(tmp, to_string(i)));
					right += to_string(i);
					right = add(right, to_string(i));
					opt += to_string(i);
					break;
				}
			}
		}
		if (right == lastright) {
			return opt;
		}
	}
	return opt;
}
cal::cal(string s) {
	stack<string> sign;
	vector<string> seq;
	vector<string> opt;
	string tmp = "";
	for (int i = 0; i < s.length();) {
		tmp += s[i];
		if (isdigit(s[i])) {
			++i;
			while (i<s.length()&&(isdigit(s[i]) || s[i] == '.')) {
				tmp += s[i];
				++i;
			}
		}
		else if (isalpha(s[i])) {
			++i;
			while (i < s.length() && (isalnum(s[i]))) {
				tmp += s[i];
				++i;
			}
		}
		else {
			++i;
		}
		if (tmp != " ") {
			seq.push_back(tmp);
		}
		tmp = "";
	}
	for (int i = 0; i < seq.size(); ++i) {
		if (dic.find(seq[i]) != dic.end()) {
			seq[i] = dic[seq[i]].value;
		}
	}
	for (int i = 0; i < seq.size();) {
		if (seq[i] == "+" || seq[i] == "-") {
			if (((i - 1) < 0 || ((!isnum(seq[i - 1]))&&seq[i-1]!=")"))) {
				if (seq[i] == "+") {
					seq[i] = "p";
					seq.insert(seq.begin() + i, "1");
				}
				else if(seq[i] == "-") {
					seq[i] = "n";
					seq.insert(seq.begin() + i, "-1");
				}
				
			}
		}
		++i;
	}
	
	for (string val : seq) {
		if (isnum(val)) {
			opt.push_back(val);
		}
		else if (val == "(") {
			sign.push("(");
		}
		else if (val == ")") {
			while (sign.top() != "(") {
				opt.push_back(sign.top());
				sign.pop();
				if (sign.empty()) {
					cout << "括號不成對\n";
					return;
				}
			}
			sign.pop();
		}
		else {
			while(!sign.empty()) {
				if (pri(val) < pri(sign.top())) {
					opt.push_back(sign.top());
					sign.pop();
				}
				else if (pri(val) == pri(sign.top())) {
					if (val == "!" || val == "^") {
						break;
					}
					else {
						opt.push_back(sign.top());
						sign.pop();
					}
				}
				else {
					break;
				}
			}
			sign.push(val);
		}
	}
	while (!sign.empty()) {
		opt.push_back(sign.top());
		sign.pop();
	}
	int lastsize;
	while (opt.size() > 1) {
		lastsize = opt.size();
		for (int i = 0; i < opt.size(); ++i) {
			if ((i + 2) < opt.size() && isnum(opt[i]) && isnum(opt[i + 1]) && !isnum(opt[i + 2])) {
				if (opt[i + 2] == "^") {
					bool sign = false;
					string temp = opt[i + 1];
					if (temp[0] == '-') {
						sign = true;
						temp.erase(temp.begin());
					}
					stringstream ss(temp);
					string f, b;
					getline(ss, f, '.');
					getline(ss, b);
					if (f == "0"&&compare("0",b)==3) {//次方為0
						if (opt[i].find('.') != -1) {
							opt[i] = "1.0";
						}
						else {
							opt[i] = "1";
						}
					}
					else {
						string tmpo = "1";
						int count = stoi(f);
						for (int z = 0; z < count; ++z) {
							tmpo = mul(tmpo, opt[i]);
						}
						
						if (!b.empty()) {
							if ((b[0] == '0') || (b[0] == '5')) {
								if (b[0] == '5') {
									tmpo = mul(tmpo, ssqrt(opt[i]));
								}
							}
							else {
								cout << "冪次為小數不是.5或整數的值\n";
								return;
							}
						}

						if (sign) {
							opt[i] = div("1",tmpo);
						}
						else {
							opt[i] = tmpo;
						}
					}
				}
				else if (opt[i + 2] == "*") {
					opt[i] = mul(opt[i], opt[i + 1]);
				}
				else if (opt[i + 2] == "/") {
					opt[i] = div(opt[i], opt[i + 1]);
				}
				else if (opt[i + 2] == "+") {
					opt[i] = add(opt[i], opt[i + 1]);
				}
				else if (opt[i + 2] == "-") {
					opt[i] = sub(opt[i], opt[i + 1]);
				}
				else if (opt[i + 2] == "n") {
					opt[i] = mul(opt[i], opt[i + 1]);
				}
				else if (opt[i + 2] == "p") {
					opt[i] = mul(opt[i], opt[i + 1]);
				}
				opt.erase(opt.begin()+ i + 2);
				opt.erase(opt.begin()+ i + 1);
				break;
			}
			if ((i + 1) < opt.size() && isnum(opt[i]) && opt[i + 1] == "!") {
				string val = opt[i];
				stringstream ss(val);
				string f, b;
				getline(ss, f, '.');
				getline(ss, b);
				bool dot = false;
				if (!b.empty()) {
					dot = true;
				}
				if (b.length() > 100) {
					b = string(b.begin(), b.begin() + 100);
				}
				val = f + "." + b;
				while (val[val.length()-1] == '0') {
					val.erase(val.end()-1);
				}
				if (val[val.length() - 1] == '.') {
					val.erase(val.end() - 1);
				}
				opt[i] = val;
				if ((opt[i].find('.') != -1)||(opt[i].find('-')!=-1)) {
					cout << "階層不為正整數";
					return;
				}
				long long int tar = stoi(opt[i]);
				string tmp = "1"; 
				for (int i = 0; i < tar; ++i) {
					tmp = mul(tmp, to_string(i + 1));
				}
				if (dot) {
					if (tmp.find('.') == -1) {
						tmp += ".0";
					}
				}
				
				
				opt.erase(opt.begin() + 1 + i);
				opt[i] = tmp;
				break;
			}
		}
		if (lastsize == opt.size()) {
			cout << "不正確的算式\n";
			return;
		}
	}
	if (opt[0].find('.') != -1) {
		Decimal d(opt[0]);
		cout << d << "\n";
		result = d.value;
	}
	else {
		Integer i(opt[0]);
		cout << i << "\n";
		result = i.value;
	}
	
}

cal::cal(string s, bool notuse) {
	stack<string> sign;
	vector<string> seq;
	vector<string> opt;
	string tmp = "";
	for (int i = 0; i < s.length();) {
		tmp += s[i];
		if (isdigit(s[i])) {
			++i;
			while (i < s.length() && (isdigit(s[i]) || s[i] == '.')) {
				tmp += s[i];
				++i;
			}
		}
		else if (isalpha(s[i])) {
			++i;
			while (i < s.length() && (isalnum(s[i]))) {
				tmp += s[i];
				++i;
			}
		}
		else {
			++i;
		}
		if (tmp != " ") {
			seq.push_back(tmp);
		}
		tmp = "";
	}
	for (int i = 0; i < seq.size(); ++i) {
		if (dic.find(seq[i]) != dic.end()) {
			seq[i] = dic[seq[i]].value;
		}
	}
	for (int i = 0; i < seq.size();) {
		if (seq[i] == "+" || seq[i] == "-") {
			if (((i - 1) < 0 || (!isnum(seq[i - 1])))) {
				if (seq[i] == "+") {
					seq[i] = "p";
					seq.insert(seq.begin() + i, "1");
				}
				else if (seq[i] == "-") {
					seq[i] = "n";
					seq.insert(seq.begin() + i, "-1");
				}

			}
		}
		++i;
	}

	for (string val : seq) {
		if (isnum(val)) {
			opt.push_back(val);
		}
		else if (val == "(") {
			sign.push("(");
		}
		else if (val == ")") {
			while (sign.top() != "(") {
				opt.push_back(sign.top());
				sign.pop();
				if (sign.empty()) {
					cout << "括號不成對\n";
					return;
				}
			}
			sign.pop();
		}
		else {
			while (!sign.empty()) {
				if (pri(val) < pri(sign.top())) {
					opt.push_back(sign.top());
					sign.pop();
				}
				else if (pri(val) == pri(sign.top())) {
					if (val == "!" || val == "^") {
						break;
					}
					else {
						opt.push_back(sign.top());
						sign.pop();
					}
				}
				else {
					break;
				}
			}
			sign.push(val);
		}
	}
	while (!sign.empty()) {
		opt.push_back(sign.top());
		sign.pop();
	}
	int lastsize;
	while (opt.size() > 1) {
		lastsize = opt.size();
		for (int i = 0; i < opt.size(); ++i) {
			if ((i + 2) < opt.size() && isnum(opt[i]) && isnum(opt[i + 1]) && !isnum(opt[i + 2])) {
				if (opt[i + 2] == "^") {
					string temp = opt[i + 1];
					stringstream ss(temp);
					string f, b;
					getline(ss, f, '.');
					getline(ss, b);
					if (f == "0" && compare("0", b) == 3) {//次方為0
						if (opt[i].find('.') != -1) {
							opt[i] = "1.0";
						}
						else {
							opt[i] = "1";
						}
					}
					else {
						string tmpo = "0";
						if (f != "0") {
							tmpo = "1";
							int count = stoi(f);
							for (int z = 0; z < count; ++z) {
								tmpo = mul(tmpo, opt[i]);
							}
						}
						if (!b.empty()) {
							if ((b[0] == '0') || (b[0] == '5')) {
								if (b[0] == '5') {
									tmpo = add(tmpo, sssqrt(opt[i]));
								}
							}
							else {
								cout << "冪次為小數不是.5或整數的值\n";
								return;
							}
						}
						opt[i] = tmpo;
					}
				}
				else if (opt[i + 2] == "*") {
					opt[i] = mul(opt[i], opt[i + 1]);
				}
				else if (opt[i + 2] == "/") {
					opt[i] = div(opt[i], opt[i + 1]);
				}
				else if (opt[i + 2] == "+") {
					opt[i] = add(opt[i], opt[i + 1]);
				}
				else if (opt[i + 2] == "-") {
					opt[i] = sub(opt[i], opt[i + 1]);
				}
				else if (opt[i + 2] == "n") {
					opt[i] = mul(opt[i], opt[i + 1]);
				}
				else if (opt[i + 2] == "p") {
					opt[i] = mul(opt[i], opt[i + 1]);
				}
				opt.erase(opt.begin() + i + 2);
				opt.erase(opt.begin() + i + 1);
				break;
			}
			if ((i + 1) < opt.size() && isnum(opt[i]) && opt[i + 1] == "!") {
				string val = opt[i];
				stringstream ss(val);
				string f, b;
				getline(ss, f, '.');
				getline(ss, b);
				bool dot = false;
				if (!b.empty()) {
					dot = true;
				}
				if (b.length() > 100) {
					b = string(b.begin(), b.begin() + 100);
				}
				val = f + "." + b;
				while (val[val.length() - 1] == '0') {
					val.erase(val.end() - 1);
				}
				if (val[val.length() - 1] == '.') {
					val.erase(val.end() - 1);
				}
				opt[i] = val;
				if ((opt[i].find('.') != -1) || (opt[i].find('-') != -1)) {
					cout << "階層不為正整數\n";
					return;
				}
				long long int tar = stoi(opt[i]);
				string tmp = "1";
				for (int i = 0; i < tar; ++i) {
					tmp = mul(tmp, to_string(i + 1));
				}
				if (dot) {
					if (tmp.find('.') == -1) {
						tmp += ".0";
					}
				}


				opt.erase(opt.begin() + 1 + i);
				opt[i] = tmp;
				break;
			}
		}
		if (lastsize = opt.size()) {
			cout << "不正確的算式\n";
			return;
		}
	}
	if (opt[0].find('.') != -1) {
		Decimal d(opt[0]);
		result = d.value;
	}
	else {
		Integer i(opt[0]);
		result = i.value;
	}

}