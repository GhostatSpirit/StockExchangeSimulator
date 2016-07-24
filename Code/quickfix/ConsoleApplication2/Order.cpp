#include <iostream>
#include <string>
#include "quickfix\Message.h"

#include "Order.h"

// default constructor that stores nothing (dangerous)
Order::Order() {
	m_clientId = "";
	m_symbol = "";
	m_owner = "";
	m_target = "";
	m_side = undefined;
	m_type = others;
	m_price = -1;
	m_quantity = -1;

	m_openQuantity = -1;
	m_executedQuantity = -1;
	m_avgExecutedPrice = -1;
	m_lastExecutedPrice = -1;
	m_lastExecutedQuantity = -1;
}


// construct an order form an existed order
Order::Order(const Order& copy) {
	m_clientId = copy.getClientID();
	m_symbol = copy.getSymbol();
	m_owner = copy.getOwner();
	m_target = copy.getTarget();
	m_side = copy.getSide();
	m_type = copy.getType();
	m_price = copy.getPrice();
	m_quantity = copy.getQuantity();

	m_openQuantity = copy.getOpenQuantity();
	m_executedQuantity = copy.getExecutedQuantity();
	m_avgExecutedPrice = copy.getAvgExecutedPrice();
	m_lastExecutedPrice = copy.getLastExecutedPrice();
	m_lastExecutedQuantity = copy.getLastExecutedQuantity();
}


// implementation of the Order constructor that constructs an order 
// from a message string
Order::Order(const std::string& orderStr) {
	// the fields we need in an order:
	/*  m_clientId(clientId), m_symbol(symbol), m_owner(owner),
	m_target(target), m_side(side), m_type(type), m_price(price),
	m_quantity(quantity)*/

	try {
		FIX::Message orderMsg(orderStr);

		// TODO: validate if the MsgType is "D = Order - Single <D>"

		// these fields are in the header:
		FIX::SenderCompID sender_obj;
		FIX::TargetCompID target_obj;

		// the other fields are in the main message:
		FIX::ClOrdID clOrdID_obj;
		FIX::OrdType ordType_obj;
		FIX::Symbol symbol_obj;
		FIX::Side side_obj;
		FIX::Price price_obj;
		FIX::OrderQty quantity_obj;

		isValid = true;


		// get SenderCompID
		if (orderMsg.getHeader().getFieldIfSet(sender_obj)) {
			m_owner = sender_obj.getValue();
		}
		else {
			isValid = false;
			cout << "Warning: field number " << sender_obj.getField() << " not set." << endl;
		}
		// get TargetCompID
		if (orderMsg.getHeader().getFieldIfSet(target_obj)) {
			m_target = target_obj.getValue();
		}
		else {
			isValid = false;
			cout << "Warning: field number " << target_obj.getField() << " not set." << endl;
		}

		// get ClOrdID
		if (orderMsg.getFieldIfSet(clOrdID_obj)) {
			m_clientId = clOrdID_obj.getValue();
		}
		else {
			cout << "Warning: field number " << clOrdID_obj.getField() << " not set." << endl;
		}

		// get OrdType
		if (orderMsg.getFieldIfSet(ordType_obj)) {
			char ordType = ordType_obj.getValue();
			switch (ordType) {
			case '1':
				m_type = market;
				break;
			case '2':
				m_type = limit;
				break;
			default:
				m_type = others;
				isValid = false;
				cout << "Warning: OrdType not implemented.(other than '1' or '2')" << endl;
			}
		}
		else {
			isValid = false;
			cout << "Warning: field number " << ordType_obj.getField() << " not set." << endl;
		}

		

		// get symbol
		if (orderMsg.getFieldIfSet(symbol_obj)) {
			m_symbol = symbol_obj.getValue();
		}
		else {
			isValid = false;
			cout << "Warning: field number " << symbol_obj.getField() << " not set. " << endl;
		}
		
		// get side
		if (orderMsg.getFieldIfSet(side_obj)) {
			char side = side_obj.getValue();
			switch (side) {
			case '1':
				m_side = buy;
				break;
			case '2':
				m_side = sell;
				break;
			default:
				throw runtime_error("Error: ordType not implemented.");
			}
		}
		else {
			isValid = false;
			cout << "Warning: field number " << side_obj.getField() << " not set." << endl;
		}

		// get Price
		if (orderMsg.getFieldIfSet(price_obj)) {
			m_price = price_obj.getValue();
		}
		else {
			isValid = false;
			cout << "Warning: field number " << price_obj.getField() << " not set." << endl;
		}

		// get Quantity
		if (orderMsg.getFieldIfSet(quantity_obj)) {
			m_quantity = quantity_obj.getValue();
		}
		else {
			isValid = false;
			cout << "Warning: field number " << quantity_obj.getField() << " not set." << endl;
		}

		// set the open quantity and other values
		m_openQuantity = m_quantity;
		m_executedQuantity = 0;
		m_avgExecutedPrice = 0;
		m_lastExecutedPrice = 0;
		m_lastExecutedQuantity = 0;
		isValid = true;
	}
	catch (exception& ex) {
		cerr << "exception in Order constructor: " << ex.what() << endl;
	}
}
