/* Created by Yang Liu, July 2016.
 * This header file defines a basic stock exchange order book of a specific symbol.
 * This order book uses two priority queues (heaps) 
 * for the SELL order book and BUY order book.
 * 
 */

#ifndef _ORDERBOOK_H
#define _ORDERBOOK_H

#include <string>
#include <iostream>
#include <vector>

#include "heap.h"			// we need the "Heap" template
#include "Order.h"			// we need the "Order" class
#include "Transaction.h"	// we need the "Transaction" class

using namespace std;

class OrderBook {
	friend std::ostream& operator<<(std::ostream& ostream, OrderBook& orderbook);
public:
	// ------------ Constructors -------------
	OrderBook(string& symbol);	// construct an OrderBook using a string symbol

	// construct an OrderBook using two vectors of Orders
	OrderBook(string& symbol, vector<Order>& sell_orders, vector<Order>& buy_orders);

	// ------------ Member Functions ---------------
	bool insert(Order& order);	// insert an order to this order book
								// return true if succeeded

	bool match(Transaction trans);		// run a match process according to the match mechanisms,
										// stores the transaction details in the Transaction object if successed
										// return true if succeeded, false if failed
	Transaction getLastTransaction() { return m_lastTransaction; }

private:
	string _symbol;				// a string stores the symbol of this order book
								// e.g. "IBM", "APPL"...

	Heap<Order> _sell_orders;	// a heap that stores all sell orders
	Heap<Order> _buy_orders;	// a heap that stores all buy orders

	bool validate_order(Order& order);

	// market info for validating orders:
	long m_tradableSellQuantity;
	long m_tradableBuyQuantity;
	double m_lastExecutedPrice;

	// market info for clients
	long m_executedQuantity;
	long m_lastExecutedQuantity;
	double m_avgExecutedPrice;

	Transaction m_lastTransaction;
	
	
};


#endif