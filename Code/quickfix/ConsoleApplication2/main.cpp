// main.cpp : Defines the entry point for the console application.
//

//#include "stdafx.h"

#include <iostream>
#include <string>

#include "quickfix\Message.h"

#include "Server.h"
#include "Order.h"
#include "OrderBook.h"
#include "Exchange.h"

using namespace std;



int main()
{

	//string msg1 = "8=FIX.4.2|9=138|35=D|34=2|49=User1|50=U1fix|52=20090206-21:13:59.324|56=FixServer|11=1233954839232|15=EUR|21=1|38=10000|40=1|44=1.25|54=1|55=EUR/USD|59=0|10=222|";
	//msg1 = replaceChar(msg1, '|', '\001');

	//string msg2 = "8=FIX.4.2|9=137|35=D|34=2|49=User2|50=U1fix|52=20090206-21:13:59.324|56=FixServer|11=1233954839232|15=EUR|21=1|38=2000|40=1|44=1.25|54=2|55=EUR/USD|59=0|10=176|";
	//msg2 = replaceChar(msg2, '|', '\001');

	////try {
	//	Exchange exchange("NYSE");
	//	exchange.process_msg(msg1);
	//	exchange.process_msg(msg2);
	//	exchange.print_orderbooks(exchange.getTradableBuy());

	//}
	//catch (exception& ex) {
	//	cerr << "error: " << ex.what() << endl;
	//}

	Server MyServer(1111); //Create server on port 100
	for (int i = 0; i < 100; i++) //Up to 100 times...
	{
		MyServer.ListenForNewConnection(); //Accept new connection (if someones trying to connect)
	}
	system("pause");
	return 0;

	//string testMsgStr = "8=FIX.4.2\0019=138\00135=D\00134=2\00149=U1par\00150=U1fix\00152=20090206-21:13:59.324\00156=FixServer\00111=1233954839232\00115=EUR\00121=1\00138=10000\00140=1\00144=1.25\00154=1\00155=EUR/USD\00159=0\00110=215\001";

	//try {
	//	Order testOrder(testMsgStr);
	//	OrderBook orderbook(testOrder.getSymbol());
	//	orderbook.insert(testOrder);

	//	cout << orderbook;

	//}
	//catch (exception& ex) {
	//	cerr << "error: " << ex.what() << endl;
	//}

	/*string temp;
	cin >> temp;*/

    return 0;
}

