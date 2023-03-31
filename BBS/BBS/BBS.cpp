// BBS.cpp : 此檔案包含 'main' 函式。程式會於該處開始執行及結束執行。
//
#pragma warning(disable : 4996) 

#include <iostream>
#include "BoardManager.h"
int main()
{
	//Post(string title, string account, string nickname, string content, string board);//新建
	//BoardManager bm();
	//Post p("我的標題","我的帳號","我的暱稱","我的內容\n可能會換行","test");
	//Post p("frAQ-Bc8W-sa1x-VPfv","test");
	//Board b("test");	
	//User u("ac", "pw", "ni");
	BoardManager bm;
	/*while (true) {
		string tmp = "";
		char c = _getch();
		tmp += c;
		if (c & 0x80) {
			tmp += _getch();
		}
		cout << tmp << "\n";
	}*/
	
}

// 執行程式: Ctrl + F5 或 [偵錯] > [啟動但不偵錯] 功能表
// 偵錯程式: F5 或 [偵錯] > [啟動偵錯] 功能表

// 開始使用的提示: 
//   1. 使用 [方案總管] 視窗，新增/管理檔案
//   2. 使用 [Team Explorer] 視窗，連線到原始檔控制
//   3. 使用 [輸出] 視窗，參閱組建輸出與其他訊息
//   4. 使用 [錯誤清單] 視窗，檢視錯誤
//   5. 前往 [專案] > [新增項目]，建立新的程式碼檔案，或是前往 [專案] > [新增現有項目]，將現有程式碼檔案新增至專案
//   6. 之後要再次開啟此專案時，請前往 [檔案] > [開啟] > [專案]，然後選取 .sln 檔案
