#include <string>
#include <iostream>
#include "Ari.h"
using namespace std;

class NumberObject {
public:
	string value;
	int id = 0;
	virtual int iden() {
		return 0;
	}
	friend ostream& operator<<(ostream& os, const NumberObject&);
};
class Signobj :public NumberObject {
public:
	Signobj(string val) {
		value = val;
	}
	virtual int iden() {
		return 3;
	}
};
class Deciaml; class Integer :public NumberObject {
public:
	Integer() {};
	Integer(const string& val);
	Integer(const char* val);
	virtual int iden() {
		return 1;
	}
	friend Integer operator+(Integer, Integer);
	friend Integer operator-(Integer, Integer);
	friend Integer operator*(Integer, Integer);
	friend Integer operator/(Integer, Integer);
	friend ostream& operator<<(ostream& os, const Integer&);
	friend istream& operator>>(istream& is, Integer&);
};

class Decimal :public NumberObject {
public:
	Decimal() {};
	Decimal(const string& val);
	Decimal(const char* val);

	virtual int iden() {
		return 2;
	}
	Decimal operator=(string s) {
		*this = Decimal(s);
		return *this;
	}
	friend Decimal operator+(Integer, Decimal);
	friend Decimal operator-(Integer, Decimal);
	friend Decimal operator*(Integer, Decimal);
	friend Decimal operator/(Integer, Decimal);
	friend Decimal operator+(Decimal, Integer);
	friend Decimal operator-(Decimal, Integer);
	friend Decimal operator*(Decimal, Integer);
	friend Decimal operator/(Decimal, Integer);
	friend Decimal operator+(Decimal, Decimal);
	friend Decimal operator-(Decimal, Decimal);
	friend Decimal operator*(Decimal, Decimal);
	friend Decimal operator/(Decimal, Decimal);
	friend ostream& operator<<(ostream& os, const Decimal&);
	friend istream& operator>>(istream& is, Decimal&);
};