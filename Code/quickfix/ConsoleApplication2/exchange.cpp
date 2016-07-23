// exchange.cpp : Defines the entry point for the console application.
//

//#include "stdafx.h"

#include <iostream>
#include <string>

#include "quickfix\Message.h"

#include "Order.h"

using namespace std;

int main()
{

	/*string testMsgStr = "8=FIX.4.2\0019=138\00135=D\00134=2\00149=U1par\00150=U1fix\00152=20090206-21:13:59.324\00156=FixServer\00111=1233954839232\00115=EUR\00121=1\00138=10000\00140=F\00144=1.25\00154=1\00155=EUR/USD\00159=0\00110=236\001";

	try {
		Order testOrder(testMsgStr);
		cout << testOrder << endl;;
	}
	catch (exception& ex) {
		cerr << "error: " << ex.what() << endl;
	}

	string temp;
	cin >> temp;
*/
	string testMsgStr = "8=FIX.4.2\0019=138\00135=D\00134=2\00149=U1par\00150=U1fix\00152=20090206-21:13:59.324\00156=FixServer\00111=1233954839232\00115=EUR\00121=1\00138=10000\00140=F\00144=1.25\00154=1\00155=EUR/USD\00159=0\00110=236\001";

	try {
		Order testOrder(testMsgStr);
		cout << testOrder << endl;;
	}
	catch (exception& ex) {
		cerr << "error: " << ex.what() << endl;
	}

	string temp;
	cin >> temp;

    return 0;
}

