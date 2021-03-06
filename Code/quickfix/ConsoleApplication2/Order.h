/* -*- C++ -*- */

/****************************************************************************
** Copyright (c) 2001-2014
**
** This file is part of the QuickFIX FIX Engine
**
** This file may be distributed under the terms of the quickfixengine.org
** license as defined by quickfixengine.org and appearing in the file
** LICENSE included in the packaging of this file.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
** See http://www.quickfixengine.org/LICENSE for licensing information.
**
** Contact ask@quickfixengine.org if any conditions of this licensing are
** not clear to you.
**
****************************************************************************/

/* Modified by Yang Liu, lykavin@homail.com.
 * This header file defines a basic stock exchange order.
 */

#ifndef _ORDERMATCH_ORDER_H
#define _ORDERMATCH_ORDER_H

#include <string>
#include <iomanip>
#include <ostream>

#include "quickfix\Message.h"
#include "quickfix\Exceptions.h"

using namespace std;

class Order
{
	friend std::ostream& operator<<( std::ostream&, const Order& );

public:
	enum Side { buy, sell, undefined };
	enum Type { market, limit, others, };

	// default constructor that stores nothing (dangerous)
	Order();

	// construct an order using a message string
	Order(const std::string& orderStr);

	// construct an order using different properties
	Order( const std::string& clientId, const std::string& symbol,
			const std::string& owner, const std::string& target,
			Side side, Type type, double price, long quantity )
	: m_clientId( clientId ), m_symbol( symbol ), m_owner( owner ),
	m_target( target ), m_side( side ), m_type( type ), m_price( price ),
	m_quantity( quantity )
	{
		m_openQuantity = m_quantity;
		m_executedQuantity = 0;
		m_avgExecutedPrice = 0;
		m_lastExecutedPrice = 0;
		m_lastExecutedQuantity = 0;
		isValid = true;
	}

	// construct an order form an existed order
	Order(const Order& copy);

	bool isValid = false;

	const std::string& getClientID() const { return m_clientId; }
	const std::string& getSymbol() const { return m_symbol; }
	const std::string& getOwner() const { return m_owner; }
	const std::string& getTarget() const { return m_target; }
	Side getSide() const { return m_side; }
	Type getType() const { return m_type; }
	double getPrice() const { return m_price; }
	long getQuantity() const { return m_quantity; }

	long getOpenQuantity() const { return m_openQuantity; }
	long getExecutedQuantity() const { return m_executedQuantity; }
	double getAvgExecutedPrice() const { return m_avgExecutedPrice; }
	double getLastExecutedPrice() const { return m_lastExecutedPrice; }
	long getLastExecutedQuantity() const { return m_lastExecutedQuantity; }

	bool isFilled() const { return m_quantity == m_executedQuantity; }
	bool isClosed() const { return m_openQuantity == 0; }

	void execute( double price, long quantity )
	{
		m_avgExecutedPrice =
			( ( quantity * price ) + ( m_avgExecutedPrice * m_executedQuantity ) )
			/ ( quantity + m_executedQuantity );

		m_openQuantity -= quantity;
		m_executedQuantity += quantity;
		m_lastExecutedPrice = price;
		m_lastExecutedQuantity = quantity;

	}
	void cancel()
	{
		m_openQuantity = 0;
	}

	// overriding operator >, <, >=, <=
	bool operator > (Order _order) {
		// cannot compare two orders if their symbols are different
		if (m_symbol != _order.getSymbol()) {
			throw logic_error("cannot compare two orders with different symbols");
		}
		if (m_price > _order.getPrice()) {
			return true;
		}
		else {
			return false;
		}
	}

	bool operator < (Order _order) {
		// cannot compare two orders if their symbols are different
		if (m_symbol != _order.getSymbol()) {
			throw logic_error("cannot compare two orders with different symbols");
		}
		if (m_price < _order.getPrice()) {
			return true;
		}
		else {
			return false;
		}
	}

	bool operator >= (Order _order) {
		// cannot compare two orders if their symbols are different
		if (m_symbol != _order.getSymbol()) {
			throw logic_error("cannot compare two orders with different symbols");
		}
		if (m_price >= _order.getPrice()) {
			return true;
		}
		else {
			return false;
		}
	}

	bool operator <= (Order _order) {
		// cannot compare two orders if their symbols are different
		if (m_symbol != _order.getSymbol()) {
			throw logic_error("cannot compare two orders with different symbols");
		}
		if (m_price <= _order.getPrice()) {
			return true;
		}
		else {
			return false;
		}
	}


private:
	std::string m_clientId;
	std::string m_symbol;
	std::string m_owner;
	std::string m_target;
	Side m_side;
	Type m_type;
	double m_price;
	long m_quantity;

	long m_openQuantity;
	long m_executedQuantity;
	double m_avgExecutedPrice;
	double m_lastExecutedPrice;
	long m_lastExecutedQuantity;
};

inline std::ostream& operator<<( std::ostream& ostream, const Order& order )
{
	ostream << "ID: " << std::setw(10) << order.getClientID() << "|"
		<< "OWNER: " << std::setw(10) << order.getOwner() << "|";

	// print side (buy/sell)
	ostream << "SIDE: " ;
	if (order.getSide() == Order::Side::buy) {
		ostream << std::setw(10) << "BUY" << "|";
	}
	else if (order.getSide() == Order::Side::sell) {
		ostream << std::setw(10) << "SELL" << "|";
	}
	else {
		ostream << std::setw(10) << "UNDEFINED" << "|";
	}

	ostream	<< "PRICE: " << std::setw(10) << order.getPrice() << "|"
		<< "QTY: " << std::setw(10) << order.getQuantity() << "|"
		<< "OPENQTY: " << std::setw(10) << order.getOpenQuantity() << "|"
		<< endl;
	
	return ostream;
}



#endif



