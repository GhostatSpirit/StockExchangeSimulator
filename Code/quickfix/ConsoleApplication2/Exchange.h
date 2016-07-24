/* Created by Yang Liu, July 2016.
* This header file defines a basic stock exchange object.
* The Exchange object maintains different OrderBooks using map.
* map<string, OrderBook> -> string _symbol, OrderBook orderbook
* 
* The Exchange object also uses a queue to cache Transaction objects
* waiting to be processed.
* 
*/
#ifndef _EXCHANGE_H
#define _EXCHANGE_H

#include <iostream>
#include <map>
#include <queue>

#include "Order.h"				// Order object
#include "quickfix\Message.h"	// Message object
#include "OrderBook.h"			// OrderBook object
#include "Transaction.h"		// Transaction object
#include "error.h"				// error()

class Exchange {
public:
	enum side {buy, sell};
	// define the map to simplify the code
	typedef map<string, OrderBook> OrderBooks;

	Exchange(const string compID);						// construct an Exchange using the give CompID

	void process_msg(const string& msg_str);			// process the given message

	void match(const string& symbol);					// run the match process in the OrderBook of the given symbol
														// until no match can be found any more
														// add all produced transaction objects to _transactions

	void process_transactions();						// process all the transactions objects in the queue

	vector<string> getTradableSell();					// get a vector<string> of tradable sell symbols
	vector<string> getTradableBuy();					// get a vector<string> of tradable buy symbols
	vector<string> getTradable();						// get a vector<string> of tradable symbols (buy/sell)

	string getCompID() { return _compID; }				// get the compID (could be name of the exchange)

	// debug sector:
	void print_orderbooks(vector<string>& tradable) {
		for (unsigned int i = 0; i < tradable.size(); ++i) {
			OrderBook* p_orderbook = find_orderbook(tradable[i]);
			if (p_orderbook == nullptr) {
				cout << "Print: cannot find the orderbook of given symbol: " << tradable[i] << endl;
				cout << endl;
			}
			else {
				cout << (*p_orderbook);
				cout << endl;
			}
		}
	}

private:
	string _compID;									// stores the CompID of the exchange
													// appears as "TargetCompID" in received FIX messages

	map<string, OrderBook> _orderbook_map;			// a map of that stores all orderbooks
	queue<Transaction> _transactions;				// a queue that caches transactions to be processed

	// find the orderbook in the exchange, return the pointer to that orderbook
	// if not found, return nullptr
	OrderBook* find_orderbook(const string target_symbol);

	void process_order(const string& msg_str);			// if the FIX message is an order, process that order
	
	
};


#endif
