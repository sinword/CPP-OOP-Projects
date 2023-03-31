#include "Ari.h"
#include <sstream>
#include <algorithm>
#include <vector>
string add(string s1, string s2) {
	bool sign1 = false, sign2 = false;
	if (s1[0] == '-') {
		sign1 = true;
		s1.erase(s1.begin());
	}
	else if (s1[0] == '+') {
		sign1 = false;
		s1.erase(s1.begin());
	}
	if (s2[0] == '-') {
		sign2 = true;
		s2.erase(s2.begin());
	}
	else if (s2[0] == '+') {
		sign2 = false;
		s2.erase(s2.begin());
	}
	if (sign1 && !sign2) {//- + +
		return sub(s2, s1);
	}
	if (!sign1 && sign2) {//+ + -
		return sub(s1, s2);
	}
	string opt = "";
	stringstream ss1(s1);
	stringstream ss2(s2);
	string f1, f2, b1, b2;
	getline(ss1, f1, '.');
	getline(ss2, f2, '.');
	getline(ss1, b1);
	getline(ss2, b2);
	while (f1.length() > 1 && f1[0] == '0') {
		f1.erase(f1.begin());
	}
	while (f2.length() > 1 && f2[0] == '0') {
		f2.erase(f2.begin());
	}
	int dotSize = max(b1.length(),b2.length());
	if (f1.length() > f2.length()) {
		f2 = string(f1.length() - f2.length(), '0') + f2;
	}
	else if (f2.length() > f1.length()) {
		f1 = string(f2.length() - f1.length(), '0') + f1;
	}
	if (b1.length() > b2.length()) {
		b2 = b2 + string(b1.length() - b2.length(), '0');
	}
	else if (b2.length() > b1.length()) {
		b1 = b1 + string(b2.length() - b1.length(), '0');
	}
	s1 = f1 + b1;
	s2 = f2 + b2;
	int n = 8;
	int lim = stoi("1" + string(n, '0'));
	int mLen = max(s1.length(), s2.length());
	mLen += n - (mLen % n);
	s1 = string(mLen - s1.length(), '0') + s1;
	s2 = string(mLen - s2.length(), '0') + s2;
	int mX = mLen / n;
	int carry = 0;
	for (int i = 0; i < mX; ++i) {
		int v1 = stoi(s1.substr(s1.length()-(i+1)*n,n));
		int v2 = stoi(s2.substr(s2.length() - (i + 1) * n, n));
		int tmp = v1 + v2 + carry;
		if (tmp >= lim) {
			carry = 1;
			tmp -= lim;
		}
		else {
			carry = 0;
		}
		string str = to_string(tmp);
		if (str.length() < n) {
			str = string(n - str.length(), '0') + str;
		}
		opt = str + opt;
	}
	if (carry) {
		opt = "1" + opt;
	}
	if (dotSize != 0) {
		opt.insert(opt.length() - dotSize, ".");
		while (opt[opt.length() - 1] == '0') {
			opt.erase(opt.end() - 1);
		}
		if (opt[opt.length() - 1] == '.') {
			opt += "0";
		}
	}
	while (opt.length() > 1&& opt[0] == '0' && opt[1] != '.') {
		opt.erase(opt.begin());
	}
	
	
	if (sign1 && sign2) {//- + -
		opt = "-" + opt;
	}
	return opt;
}
string sub(string s1, string s2) {
	string opt = "";
	stringstream ss1(s1);
	stringstream ss2(s2);
	string f1, f2, b1, b2;
	getline(ss1, f1, '.');
	getline(ss2, f2, '.');
	getline(ss1, b1);
	getline(ss2, b2);
	int comp;
	bool sign1 = false, sign2 = false;
	if (f1[0] == '-') {
		sign1 = true;
		s1.erase(s1.begin());
		f1.erase(f1.begin());
	}
	else if (f1[0] == '+') {
		s1.erase(s1.begin());
		f1.erase(f1.begin());
		sign1 = false;
	}
	if (f2[0] == '-') {
		sign2 = true;
		s2.erase(s2.begin());
		f2.erase(f2.begin());
	}
	else if (f2[0] == '+') {
		s2.erase(s2.begin());
		f2.erase(f2.begin());
		sign2 = false;
	}
	while (f1.length() > 1 && f1[0] == '0') {
		f1.erase(f1.begin());
	}
	while (f2.length() > 1 && f2[0] == '0') {
		f2.erase(f2.begin());
	}
	bool dot = false;
	if (b1.empty() && b2.empty()) {//是否有小數點
		dot = false;
	}
	else {
		dot = true;
		if (b1.length() > b2.length()) {
			b2 = b2 + string(b1.length() - b2.length(), '0');
		}
		else if (b2.length() > b1.length()) {
			b1 = b1 + string(b2.length() - b1.length(), '0');
		}
	}

	if (f1.length() > f2.length()) {
		comp = 1;
	}
	else if (f1.length() < f2.length()) {
		comp = 2;
	}
	else if (f1.length() == f2.length()) {
		bool compF = false;
		int count = 0;
		while (count<f1.length()) {
			if (f1[count] > f2[count]) {
				comp = 1;
				compF = true;
				break;
			}
			else if (f1[count] < f2[count]) {
				comp = 2;
				compF = true;
				break;
			}
			else if (f1[count] == f2[count]) {
				++count;
			}
		}
		if (!compF) {
			if (!dot) {
				comp = 3;
			}
			else {
				int dotCount = 0;
				while (true) {
					if (b1[dotCount] > b2[dotCount]) {
						comp = 1;
						compF = true;
						break;
					}
					else if (b1[dotCount] < b2[dotCount]) {
						comp = 2;
						compF = true;
						break;
					}
					else if (b1[dotCount] == b2[dotCount]) {
						++dotCount;
					}
					if (dotCount >= b1.length() && dotCount >= b2.length()) {
						comp = 3;
						compF = true;
						break;
					}
					else if (dotCount >= b1.length()) {
						comp = 2;
						compF = true;
						break;
					}
					else if (dotCount >= b2.length()) {
						comp = 1;
						compF = true;
						break;
					}
				}
			}
		}
	}

	if (!sign1&&sign2) { //+ - -
		return add(s1, s2);
	}
	else if (sign1 && sign2) {//- - -
		return sub(s2, s1);
	}
	else if (sign1 && !sign2) {//- - +
		return "-" + add(s1, s2);
	}
	else if (!sign1 && !sign2) {//+ - +
		if (comp == 3) {
			if (dot) {
				return "0.0";
			}
			else {
				return "0";
			}
		}
		else if (comp == 2) {
			return "-" + sub(s2, s1);
		}
		else if (comp == 1) {
			string opt = "";
			int dotSize = max(b1.length(), b2.length());//最大小數點長度
			b1 = string(dotSize - b1.length(), '0') + b1;
			b2 = string(dotSize - b2.length(), '0') + b2;
			int decSize = max(f1.length(), f2.length());
			f1 = string(decSize - f1.length(), '0') + f1;
			f2 = string(decSize - f2.length(), '0') + f2;
			int c = b1.length() - 1;
			int borrow = 0;
			if (dot) {
				while (c >= 0) {
					int tmp = (b1[c] - '0') - (b2[c] - '0') - borrow;
					if (tmp < 0) {
						borrow = 1;
						tmp += 10;
					}
					else {
						borrow = 0;
					}
					opt = to_string(tmp) + opt;
					--c;
				}
				opt = "." + opt;
			}
			c = f1.length() - 1;
			while (c >= 0) {
				int tmp = (f1[c] - '0') - (f2[c] - '0') - borrow;
				if (tmp < 0) {
					borrow = 1;
					tmp += 10;
				}
				else {
					borrow = 0;
				}
				opt = to_string(tmp) + opt;
				--c;
			}
			if (dot) {
				int count = 0;
				while (true) {
					if (opt[count + 1] != '.') {
						if (opt[count] == '0') {
							opt.erase(opt.begin());
						}
						else {
							break;
						}
					}
					else {
						break;
					}
				}
				while (opt[opt.length() - 1] == '0') {
					opt.erase(opt.end()-1);
				}
			}
			else {
				while (opt.length() > 1 && opt[0] == '0') {
					opt.erase(opt.begin());
				}
			}
			return opt;
		}
	}
}
string ShiftString(string num, int len) {
	if (num == "0") {
		return num;
	}//if
	for (int i = 0; i < len; ++i) {
		num += "0";
	}//for
	return num;
}
string mul(string s1, string s2) {
	bool sign1 = false, sign2 = false;
	if (s1[0] == '-') {
		sign1 = true;
		s1.erase(s1.begin());
	}
	else if (s1[0] == '+') {
		sign1 = false;
		s1.erase(s1.begin());
	}
	if (s2[0] == '-') {
		sign2 = true;
		s2.erase(s2.begin());
	}
	else if (s2[0] == '+') {
		sign2 = false;
		s2.erase(s2.begin());
	}
	string opt = "";
	stringstream ss1(s1);
	stringstream ss2(s2);
	string f1, f2, b1, b2;
	getline(ss1, f1, '.');
	getline(ss2, f2, '.');
	getline(ss1, b1);
	getline(ss2, b2);
	while (f1.length() > 1 && f1[0] == '0') {
		f1.erase(f1.begin());
	}
	while (f2.length() > 1 && f2[0] == '0') {
		f2.erase(f2.begin());
	}
	if (f1.length() > f2.length()) {
		f2 = string(f1.length() - f2.length(), '0') + f2;
	}
	else if (f2.length() > f1.length()) {
		f1 = string(f2.length() - f1.length(), '0') + f1;
	}
	if (b1.length() > b2.length()) {
		b2 = b2 + string(b1.length() - b2.length(), '0');
	}
	else if (b2.length() > b1.length()) {
		b1 = b1 + string(b2.length() - b1.length(), '0');
	}
	int dotSize = b1.length() + b2.length();
	s1 = f1 + b1;
	s2 = f2 + b2;
	int n = 8;
	int lim = stoi("1" + string(n, '0'));
	int mLen = max(s1.length(), s2.length());
	mLen += n - (mLen % n);
	s1 = string(mLen - s1.length(), '0') + s1;
	s2 = string(mLen - s2.length(), '0') + s2;
	int mX = mLen / n;
	vector<long long int> v,v1,v2;
	v1.resize(mX);
	v2.resize(mX);
	v.resize(mX*2+1);
	for (int i = 0; i < v.size(); ++i) {
		v[i] = 0;
	}
	for (int i = 0; i < mX; ++i) {
		v1[i] = stoi(s1.substr(s1.length() - (i + 1) * n, n));
		v2[i] = stoi(s2.substr(s2.length() - (i + 1) * n, n));
	}
	for (int i = 0; i < mX; ++i) {
		for (int j = 0; j < mX; ++j) {
			v[i+j] += v1[i] * v2[j];
		}
		for (int z = 0; z < v.size(); ++z) {
			if (v[z] >= lim) {
				v[z + 1] += v[z] / lim;
				v[z] = v[z] % lim;
			}
		}
	}
	for (int i = 0; i < v.size(); ++i) {
		string tmp = to_string(v[i]);
		opt = string(n - tmp.length(), '0') + tmp + opt;
	}
	if (dotSize!=0) {
		opt.insert(opt.length() - dotSize, ".");
		while (opt[opt.length() - 1] == '0') {
			opt.erase(opt.end() - 1);
		}
		if (opt[opt.length() - 1] == '.') {
			opt += "0";
		}
		if (dotSize >= 100) {
			string adjust = "0." + string(102, '0') + "1";
			opt = add(opt, adjust);
		}
	}
	while (opt.length() > 1 && opt[0] == '0' && opt[1] != '.') {
		opt.erase(opt.begin());
	}
	if (sign1 ^ sign2) {
		opt = "-" + opt;
	}




	return opt;
}
//string KaratsubaMultiply(string s1,string s2) {
//	if (s1.length() > s2.length()) {
//		s2 = string(s1.length() - s2.length(), '0') + s2;
//	}
//	else if (s2.length() > s1.length()) {
//		s1 = string(s2.length() - s1.length(), '0') + s1;
//	}
//	int len = s1.length();
//	if (len == 0) {
//		return 0;
//	}
//	if (len == 1) {
//		return to_string((s1[0] - '0') * (s2[0] - '0'));
//	}
//	int mid = len / 2;
//
//	string x1 = s1.substr(0, mid);
//	string x0 = s1.substr(mid, len - mid);
//	// Find the first half and second half of second string
//	string y1 = s2.substr(0, mid);
//	string y0 = s2.substr(mid, len - mid);
//	// Recursively computer
//	string z0 = mul(x0, y0);
//	string z1 = mul(add(x1, x0), add(y1, y0));
//	string z2 = mul(x1, y1);
//	// (z2*10^(2*m))+((z1-z2-z0)*10^(m))+(z0)
//	// z2*10^(2*m)
//	string r1 = ShiftString(z2, 2 * (len - mid));
//	// (z1-z2-z0)*10^(m)
//	string r2 = ShiftString(sub(sub(z1, z2), z0), len - mid);
//	return  add(add(r1, r2), z0);
//}
//string mul(string s1, string s2) {
//	bool sign1 = false, sign2 = false;
//	if (s1[0] == '-') {
//		sign1 = true;
//		s1.erase(s1.begin());
//	}
//	else if (s1[0] == '+') {
//		sign1 = false;
//		s1.erase(s1.begin());
//	}
//	if (s2[0] == '-') {
//		sign2 = true;
//		s2.erase(s2.begin());
//	}
//	else if (s2[0] == '+') {
//		sign2 = false;
//		s2.erase(s2.begin());
//	}
//	stringstream ss1(s1);
//	stringstream ss2(s2);
//	string f1, b1, f2, b2;
//	getline(ss1, f1, '.');
//	getline(ss2, f2, '.');
//	getline(ss1, b1);
//	getline(ss2, b2);
//	while (f1.length() > 1 && f1[0] == '0') {
//		f1.erase(f1.begin());
//	}
//	while (f2.length() > 1 && f2[0] == '0') {
//		f2.erase(f2.begin());
//	}
//	int dotSize = b1.length() + b2.length();
//	s1 = f1 + b1;
//	s2 = f2 + b2;
//	//while (s1[0] == '0') {
//	//	s1.erase(s1.begin());
//	//	//s1 += "0";
//	//}
//	//while (s2[0] == '0') {
//	//	s2.erase(s2.begin());
//	//	//s2 += "0";
//	//}
//	string opt = KaratsubaMultiply(s1, s2);
//	if (dotSize > 0) {
//		if (opt.length()<=dotSize) {
//			opt = "0." + string(dotSize - opt.length(), '0') + opt;
//		} 
//		else {
//			opt.insert(opt.length() - dotSize, ".");
//		}
//	}
//	if (sign1 ^ sign2) {
//		opt = "-" + opt;
//	}
//	
//	return opt;
//}
int compare(string s1, string s2) {
	stringstream ss1(s1);
	stringstream ss2(s2);
	string f1, b1, f2, b2;
	getline(ss1, f1, '.');
	getline(ss2, f2, '.');
	getline(ss1, b1);
	getline(ss2, b2);
	bool dot = true;
	if (b1.empty() && b2.empty()) {
		dot = false;
	}
	else {
		dot = true;
		if (b1.length() > b2.length()) {
			b2 = b2 + string(b1.length() - b2.length(), '0');
		}
		else if (b2.length() > b1.length()) {
			b1 = b1 + string(b2.length() - b1.length(), '0');
		}
	}
	if (f1[0] == '-' || f1[0] == '+') {
		f1.erase(f1.begin());
	}
	if (f2[0] == '-' || f2[0] == '+') {
		f2.erase(f2.begin());
	}
	
	while (f1.length()>1&&f1[0] == '0') {
		f1.erase(f1.begin());
	}

	while (f2.length() > 1 && f2[0] == '0') {
		f2.erase(f2.begin());
	}


	if (f1.length() > f2.length()) {
		return 1;
	}
	else if (f1.length() < f2.length()) {
		return 2;
	}
	else if (f1.length() == f2.length()) {
		bool compF = false;
		int count = 0;
		while (count < f1.length()) {
			if (f1[count] > f2[count]) {
				return 1;
			}
			else if (f1[count] < f2[count]) {
				return 2;

			}
			else if (f1[count] == f2[count]) {
				++count;
			}
		}
		if (!compF) {
			if (!dot) {
				return 3;
			}
			else {
				int dotCount = 0;
				while (true) {
					if (b1[dotCount] > b2[dotCount]) {
						return 1;
					}
					else if (b1[dotCount] < b2[dotCount]) {
						return 2;
					}
					else if (b1[dotCount] == b2[dotCount]) {
						++dotCount;
					}
					if (dotCount >= b1.length() && dotCount >= b2.length()) {
						return 3;
					}
					else if (dotCount >= b1.length()) {
						return 2;
					}
					else if (dotCount >= b2.length()) {
						return 1;
					}
				}
			}
		}
	}
}
	string div(string s1,string s2) {
	string opt="";
	bool sign1 = false, sign2 = false;
	if (s1[0] == '-') {
		sign1 = true;
		s1.erase(s1.begin());
	}
	else if (s1[0] == '+') {
		sign1 = false;
		s1.erase(s1.begin());
	}
	if (s2[0] == '-') {
		sign2 = true;
		s2.erase(s2.begin());
	}
	else if (s2[0] == '+') {
		sign2 = false;
		s2.erase(s2.begin());
	}
	stringstream ss1(s1);
	stringstream ss2(s2);
	string f1, b1, f2, b2;
	getline(ss1, f1, '.');
	getline(ss2, f2, '.');
	getline(ss1, b1);
	getline(ss2, b2);
	int dotSize = b1.length() - b2.length();
	bool dot = false;
	if (!b1.empty() || !b2.empty()) {
		dot = true;
	}
	s1 = f1 + b1;
	s2 = f2 + b2;
	while (s1[0] == '0' && s1.length() > 1) {
		s1.erase(s1.begin());
	}
	while (s2[0] == '0' && s2.length() > 1) {
		s2.erase(s2.begin());
	}
	if (s2.length() > s1.length()) {
		dotSize += s2.length() - s1.length();
		s1 += string(s2.length() - s1.length(), '0');
	}
	s1 = "0" + s1;
	int precision = 105;
	s1 += string(precision, '0');
	int len = s2.length()+1;
	int cur = 0;
	while (s1.length()>=len) {
		string tmp = string(s1.begin(),s1.begin()+len);
		s1 = string(s1.begin() + len, s1.end());
		string lmp, mp = "0";
		for (int i = 1; i <= 10; ++i) {
			lmp = mp;
			mp = mul(s2, to_string(i));
			int res = compare(tmp, mp);
			if (res == 2) {
				opt += to_string(i - 1);
				string fix = sub(tmp, lmp);
				s1 = string((len-1)-fix.length(),'0')+ fix + s1;
				break;
			}
			else if (res == 3) {
				s1 = string(len - 1, '0')+s1;
				opt += to_string(i);
				break;
			}
		}
	}

	string lmp, mp;
	for (int i = 1; i <= 10; ++i) {
		lmp = mp;
		mp = mul(s2, to_string(i));
		int res = compare(s1, mp);
		if (res == 2) {
			opt += to_string(i - 1);
			break;
		}
		else if (res == 3) {
			opt += to_string(i);
			break;
		}
	}


	int shift = precision + dotSize;
	if (opt.length() <= shift) {
		opt = "0." + string(shift+1 - opt.length(), '0') + opt;
	}
	else {
		opt.insert(opt.length()-1 - shift, ".");
	}
	while (opt[1]!='.' && opt[0] == '0') {
		opt.erase(opt.begin());
	}

	if (opt[opt.length() - 1] == '.') {
		opt.erase(opt.end() - 1);
	}
	if (sign1 ^ sign2) {
		opt = "-" + opt;
	}
	if (!dot) {
		stringstream ss(opt);
		string f, b;
		getline(ss, f, '.');
		getline(ss, b);
		opt = f;
	}
	return opt;
}




//string FETdiv(string s1, string s2) {
//	int targetDig = s1.length()+5;
//	string x0;
//	if (s2[0]=='1') {
//		x0 = "1";
//	}
//	else {
//		x0 = to_string((long double)1 / (s2[0] - '0'));
//		while (x0[x0.length() - 1] == '0') {
//			x0.erase(x0.end() - 1);
//		}
//	}
//
//	if (s2.length() != 1) {
//		string shift = "0.";
//		int plusZero = s2.length() - 2;
//		if (plusZero > 0) {
//			shift += string(plusZero, '0');
//		}
//		shift += "1";
//		x0 = mul(x0, shift);
//	}
//
//	string xN;
//	while (true) {
//		xN = mul(sub("2", mul(x0, s2)), x0);
//		if (x0.length() >= targetDig + 2) {
//			bool flag = true;
//			for (int i = 0; i < targetDig; ++i) {
//				if (xN[2 + i] != x0[2 + i]) {
//					flag = false;
//					break;
//				}
//			}
//			if (flag) {
//				break;
//			}
//		}
//		x0 = xN;
//	}
//	return mul(s1, xN);
//
//}
//string div(string s1, string s2) {
//	if (s2 == "1") {
//		return s1;
//	}
//	stringstream ss1(s1);
//	stringstream ss2(s2);
//	string f1, f2, b1, b2;
//	getline(ss1, f1, '.');
//	getline(ss2, f2, '.');
//	getline(ss1, b1);
//	getline(ss2, b2);
//	bool dotSize = b1.length() - b2.length();
//	string opt = FETdiv(f1 + b1, f2 + b2);
//	if (dotSize > 0) {
//		opt.insert(opt.length() - dotSize, ".");
//	}
//	else if (dotSize < 0) {
//		opt += string(abs(dotSize), '0');
//	}
//	return opt;
//}