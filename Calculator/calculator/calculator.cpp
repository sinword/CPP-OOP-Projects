#include "cal.h"
#include <iostream>
#include <vector>

int main()
{
	//cout << sssqrt("3.1415926") << "\n";
	/*cout << add("10.0", "10.0") << "\n";
	cout << sub("10.0", "10.0") << "\n";
	cout << mul("10.0", "10.0") << "\n";
	cout << div("10.0", "10.0") << "\n";*/
	/*cal("1+++++1--+-+---1");
	cal("1.5 + 3 * ( -(-5) )");
	cal("1 / 7 * 3 * -(6 + 1)");
	cal("-(-(-(-(-(-(((1))))))))");

	cal("123456^80");*/
	//cal("123456789^0.5");
	//cal("((2^3)^(0.5))^(2*2)");

	//cal("100!");
	//cal("(1 + 3 * 8 - -(4 + 6 * 7))!");
	//cal("5!!");

	//cal("1 / 7.0 * 3 * -(6 + 1)");
	//cal("1/3.0");
	//cal("((2^3)^(1/2.0))^(2*0.5)");
	//cal("(1/3.0 * 6)!");
	




	while (true) {
		cout << "Availabe option Set cal assign:";
		string cmd;
		cin >> cmd;
		if (cmd == "Set") {
			string type;
			string name;
			string val;
			cin >> type >> name;
			string sxx;
			getline(cin, sxx, '=');
			getline(cin, val);
			if (type == "Integer") {
				cal::dic[name] = Integer(cal(val,true).result);
			}
			else if (type == "Decimal") {
				cal::dic[name] = Decimal(cal(val,true).result);
			}
		}
		else if (cmd == "cal") {
			cin.get();
			string val;
			getline(cin, val);
			cal tmp(val);
		}
		else if (cmd == "assign") {
			string name;
			cin >> name;
			string val;
			string sxx;
			getline(cin, sxx, '=');
			getline(cin, val);
			if (cal::dic[name].id == 1) {
				cal::dic[name] = Integer(cal(val, true).result);
			}
			else if (cal::dic[name].id == 2) {
				cal::dic[name] = Decimal(cal(val, true).result);
			}
			
		}
	}
}

