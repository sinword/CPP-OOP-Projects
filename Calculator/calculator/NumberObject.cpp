#include "NumberObject.h"
#include <sstream>
Integer::Integer(const string& val) {
	id = 1;
	stringstream ss(val);
	string f;
	getline(ss, f, '.');
	value = f;
}
Integer::Integer(const char* v) {
	string val = v;
	id = 1;
	stringstream ss(val);
	string f;
	getline(ss, f, '.');
	value = f;
}

Decimal::Decimal(const string& val) {
	id = 2;
	stringstream ss(val);
	string f,b;
	getline(ss, f, '.');
	getline(ss, b);
	string opt;
	opt = f +"."+ b;
	if (b.empty()) {
		opt = f + ".0";
	}
	value = opt;
}
Decimal::Decimal(const char* v) {
	string val = v;
	id = 2;
	stringstream ss(val);
	string f, b;
	getline(ss, f, '.');
	getline(ss, b);
	string opt;
	opt = f +"."+ b;
	if (b.empty()) {
		opt = f + ".0";
	}
	value = opt;
}


ostream& operator<<(ostream& os, const NumberObject& n) {
	if (typeid(n) == typeid(Integer)) {
		Integer i(n.value);
		cout << i;
	}
	else if (typeid(n) == typeid(Decimal)) {
		Decimal d(n.value);
		cout << d;
	}
	return os;
}
ostream& operator<<(ostream& os, const Integer& i) {
	string opt = i.value;
	if (opt == "-0") {
		opt = "0";
	}
	cout << opt;
	return os;
}
istream& operator>>(istream& is, Integer& i) {
	string tmp;
	is >> tmp;
	i = Integer(tmp);
	return is;
}
ostream& operator<<(ostream& os, const Decimal& d) {
	string adjust = "0." + string(100, '0') + "1";
	bool sign = false;
	string val = d.value;
	if (val[0] == '-') {
		sign = true;
		val.erase(val.begin());
	}
	string opt = add(val, adjust);
	stringstream ss(opt);
	string f, b;
	getline(ss, f, '.');
	getline(ss, b);
	b = string(b.begin(), b.begin() + 100);
	string tmp =  f+"."+b;
	if (sign) {
		tmp = "-" + tmp;
	}
	bool zeroFlag = true;
	for (char c : tmp) {
		if (c != '0' && c != '.') {
			zeroFlag = false;
			break;
		}
	}
	if (zeroFlag && ((tmp[0] == '+') || (tmp[0] == '-'))) {
		tmp.erase(tmp.begin());
	}
	cout << tmp;
	return os;
}
istream& operator>>(istream& is, Decimal& d) {
	string tmp;
	is >> tmp;
	d = Decimal(tmp);
	return is;
}




Integer operator+(Integer a, Integer b) {
	return Integer(add(a.value, b.value));
}
Integer operator-(Integer a, Integer b) {
	return Integer(sub(a.value, b.value));
}
Integer operator*(Integer a, Integer b) {
	return Integer(mul(a.value, b.value));
}
Integer operator/(Integer a, Integer b) {
	return Integer(div(a.value, b.value));
}
Decimal operator+(Integer a, Decimal b) {
	return Decimal(add(a.value, b.value));
}
Decimal operator-(Integer a, Decimal b) {
	return Decimal(sub(a.value, b.value));
}
Decimal operator*(Integer a, Decimal b) {
	return Decimal(mul(a.value, b.value));
}
Decimal operator/(Integer a, Decimal b) {
	return Decimal(div(a.value, b.value));
}
Decimal operator+(Decimal a, Integer b) {
	return Decimal(add(a.value, b.value));
}
Decimal operator-(Decimal a, Integer b) {
	return Decimal(sub(a.value, b.value));
}
Decimal operator*(Decimal a, Integer b) {
	return Decimal(mul(a.value, b.value));
}
Decimal operator/(Decimal a, Integer b) {
	return Decimal(div(a.value, b.value));
}
Decimal operator+(Decimal a, Decimal b) {
	return Decimal(add(a.value, b.value));
}
Decimal operator-(Decimal a, Decimal b) {
	return Decimal(sub(a.value, b.value));
}
Decimal operator*(Decimal a, Decimal b) {
	return Decimal(mul(a.value, b.value));
}
Decimal operator/(Decimal a, Decimal b) {
	return Decimal(div(a.value, b.value));
}

