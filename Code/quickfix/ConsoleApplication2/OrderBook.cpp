
#include <algorithm>		// std::max()

#include "OrderBook.h"
#include "error.h"

using namespace std;

OrderBook::OrderBook(string& symbol) {
	_symbol = symbol;
	_sell_orders.setSmallerTop();
	_buy_orders.setBiggerTop();
	
	// initializing variables:
	m_tradableSellQuantity = 0;
	m_tradableBuyQuantity = 0;
	m_lastExecutedPrice = 0;
	m_executedQuantity = 0;
	m_lastExecutedQuantity = 0;
	m_avgExecutedPrice = 0;
}

OrderBook::OrderBook(const string& symbol) {
	_symbol = symbol;
	_sell_orders.setSmallerTop();
	_buy_orders.setBiggerTop();

	// initializing variables:
	m_tradableSellQuantity = 0;
	m_tradableBuyQuantity = 0;
	m_lastExecutedPrice = 0;
	m_executedQuantity = 0;
	m_lastExecutedQuantity = 0;
	m_avgExecutedPrice = 0;
}

OrderBook::OrderBook(string& symbol, vector<Order>& sell_orders, vector<Order>& buy_orders) {
	_symbol = symbol;
	// using constructor to heapify using Floyd(the only way)
	if (sell_orders.size() > 0) {
		Heap<Order> temp_sell_orders(sell_orders, true);
		_sell_orders = temp_sell_orders;
	}
	if (buy_orders.size() > 0) {
		Heap<Order> temp_buy_orders(buy_orders, false);
		_buy_orders = temp_buy_orders;
	}
	// reset whether the smaller or bigger element should be on the top,
	// just to be sure
	_sell_orders.setSmallerTop();
	_buy_orders.setBiggerTop();

	// initializing variables:
	m_tradableSellQuantity = 0;
	m_tradableBuyQuantity = 0;

	// for calculating average share price:
	double m_sellGrossValue = 0;
	double m_buyGrossValue = 0;

	for (unsigned int i = 0; i < sell_orders.size(); ++i) {
		Order& temp_order = sell_orders[i];
		m_tradableSellQuantity += temp_order.getQuantity();
		m_buyGrossValue += temp_order.getQuantity() * temp_order.getPrice();
	}
	for (unsigned int i = 0; i < buy_orders.size(); ++i) {
		Order& temp_order = buy_orders[i];
		m_tradableBuyQuantity += temp_order.getQuantity();
		m_sellGrossValue += temp_order.getQuantity() * temp_order.getPrice();
	}

	// calculating opening price (naive version) and make it lastExecutedPrice
	if (sell_orders.size() == 0) {
		m_lastExecutedPrice = m_buyGrossValue / m_tradableBuyQuantity;
	}
	else if (buy_orders.size() == 0) {
		m_lastExecutedPrice = m_sellGrossValue / m_tradableSellQuantity;
	}
	else {
		m_lastExecutedPrice = (m_buyGrossValue / m_tradableBuyQuantity
			+ m_sellGrossValue / m_tradableSellQuantity) / 2.0;
	}

	// initializing the last three variables
	m_executedQuantity = 0;
	m_lastExecutedQuantity = 0;
	m_avgExecutedPrice = 0;
}


bool OrderBook::insert(Order& order) {
	if (validate_order(order) != true) {
		return false;
		// TODO: send a message back to client stating why the order is rejected
	}

	if (order.getSide() == Order::Side::sell) {
		_sell_orders.insert(order);
		m_tradableSellQuantity += order.getQuantity();
		return true;
	}
	else if (order.getSide() == Order::Side::buy) {
		_buy_orders.insert(order);
		m_tradableBuyQuantity += order.getQuantity();
		return true;
	}
	else {
		return false;
		// TODO : send a message saying that this order side type is not supported
	}
}

bool OrderBook::validate_order(Order& order) {
	try {
		if (order.getSymbol() != _symbol)
			error("symbol not match");
		if (order.getType() == Order::Type::others)
			error("order type not supported");

		// TODO: add other if statements to judge whether the order is valid
	}
	catch(exception& ex){
		cerr << "order added to: " << _symbol << " not valid: " 
			 << ex.what() << endl;
		return false;
	}
	return true;
}

Transaction OrderBook::match() {
	Transaction trans;
	if (_sell_orders.size() == 0 || _buy_orders.size() == 0) {
		return trans;
	}

	// get the sell order of the lowest price,
	// and the buy order of the highest price
	Order& lowestSellOrder = _sell_orders.getTop();
	Order& highestBuyOrder = _buy_orders.getTop();

	double sellPrice = lowestSellOrder.getPrice();
	double buyPrice = highestBuyOrder.getPrice();

	// if they do not match, return false
	if (sellPrice > buyPrice) {
		return trans;
	}
	else {
		// see which one has the larger quantity

		
		long sellQty = lowestSellOrder.getOpenQuantity();
		long buyQty = highestBuyOrder.getOpenQuantity();


		bool sellLess = (sellQty <= buyQty);
		long executeQty;
		if (sellLess) {
			executeQty = sellQty;
		}
		else {
			executeQty = buyQty;
		}

		// calculate the executePrice
		// (naive version) executePrice = middle(sellPrice, buyPrice)
		double executePrice = (sellPrice + buyPrice) / 2.0;

		// execute both order with the executeQty and executePrice
		lowestSellOrder.execute(executePrice, executeQty);
		highestBuyOrder.execute(executePrice, executeQty);
		
		// send the two executed orders to the Transaction object
		trans.sellOrder = lowestSellOrder;
		trans.buyOrder = highestBuyOrder;
		trans.validate();

		// check and delete the order(s) with 0 quantity
		if (sellLess) {
			_sell_orders.delTop();
			if (highestBuyOrder.isFilled()) {
				_buy_orders.delTop();
			}
		}
		else {
			_buy_orders.delTop();
		}

		// update the m_ values
		m_tradableSellQuantity -= executeQty;
		m_tradableBuyQuantity -= executeQty;
		m_lastExecutedPrice = executePrice;

		// market info for clients
		m_avgExecutedPrice =   (m_avgExecutedPrice * m_executedQuantity
								+ executePrice * executeQty) 
							   / (executeQty + m_executedQuantity);
		m_executedQuantity += executeQty;
		m_lastExecutedQuantity = executeQty;
		
		// update last transaction
		m_lastTransaction = trans;

		return trans;
	}
	

}

// overloading << operator to print out the order book
std::ostream & operator<<(std::ostream & ostream, OrderBook & orderbook)
{
	const string marker = "> ";
	// step 1: print out the symbol:
	ostream << marker << orderbook._symbol << " ORDERBOOK" << endl;
	
	// step 2: print out the sell orders line by line:
	ostream << marker << "SELL ORDERS" << endl;
	// print out sell orders using iterator
	for (Heap<Order>::const_iterator it = orderbook._sell_orders.begin();
		it != orderbook._sell_orders.end(); ++it) {
		ostream << marker << (*it);
	}

	// print out a new line
	ostream << endl;

	// step 3: print out the buy orders line by line:
	ostream << marker << "BUY ORDERS" << endl;
	for (Heap<Order>::const_iterator it = orderbook._buy_orders.begin();
		it != orderbook._buy_orders.end(); ++it) {
		ostream << marker << (*it);
	}

	// print out a new line
	//ostream << endl;

	// return the ostream
	return ostream;
}
