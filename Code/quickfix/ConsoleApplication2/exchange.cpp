

#include "Exchange.h"

#include <fstream>

using namespace std;

Exchange::Exchange(const string compID)
	:_compID(compID){
	// other things needed for this constructor
	log_file_name = "exchange.txt";
}

void Exchange::process_msg(const string & msg_str)
{	
	FIX::MsgType msgtype;

	// create the message object, check the MsgType and destroy it
	try{
		FIX::Message message(msg_str);

		// get the MsgType, and if failed, throw exception
		if (!message.getHeader().getFieldIfSet(msgtype)) {
			error("Exchange: process_msg: MsgType not found");
		}
	}
	catch (std::exception& ex) {
		cout << "! Error: " << ex.what() << endl;
	}

	// process the order according to its type
	if (msgtype.getString() == FIX::MsgType_NewOrderSingle) {
		process_order(msg_str);
		return;
	}
	else {
		cerr << "! Warning: MsgType not supported. " << endl;
		cerr << "! Original message: " << msg_str << endl;
	}
}

void Exchange::match(const string & symbol)
{
	// try to find the orderbook in the map
	OrderBook* p_orderbook = find_orderbook(symbol);

	// if cannot find, log and abort
	if (p_orderbook == nullptr) {
		cerr << "Warning: cannot find orderbook of symbol: " << symbol << endl;
		return;
	}

	// run the match function again and again, until no match can be found
	bool has_match = true;
	while (has_match) {
		Transaction trans = p_orderbook->match();
		// if we succeed to match
		if (trans.isValid()) {
			// add the transaction object to the queue
			_transactions.push(trans);
		}
		else {
			has_match = false;
		}
	}

	return;

}

void Exchange::process_transactions()
{
	// while the transactions queue is not empty...
	while (!(_transactions.empty())) {
		// TODO: replace this with writing log files into a .log file
		Transaction& trans = _transactions.front();
		// show the transaction on the console
		cout << trans;
		// write the transaction to a log file
		ofstream ofs(log_file_name, ios_base::out|ios_base::app);
		if (!ofs) {
			error("cannot open log file");
			cout << "failed to open log file";
		}

		ofs << trans;
		ofs.close();

		_transactions.pop();
	}
}

vector<string> Exchange::getTradableSell()
{
	vector<string> tradable;
	OrderBooks::iterator iter;
	// iterate through the map
	for (iter = _orderbook_map.begin(); iter != _orderbook_map.end(); ++iter) {
		OrderBook& orderbook = iter->second;
		// if the sell side of the orderbook is not empty, add the symbol to the vector
		if (!orderbook.isSellEmpty()) {
			tradable.push_back(iter->first);
		}
	}

	return tradable;

	
}

vector<string> Exchange::getTradableBuy()
{
	vector<string> tradable;
	OrderBooks::iterator iter;
	// iterate through the map
	for (iter = _orderbook_map.begin(); iter != _orderbook_map.end(); ++iter) {
		OrderBook& orderbook = iter->second;
		// if the buy side of the orderbook is not empty, add the symbol to the vector
		if (!orderbook.isBuyEmpty()) {
			tradable.push_back(iter->first);
		}
	}

	return tradable;
}

vector<string> Exchange::getTradable()
{
	vector<string> tradable;
	OrderBooks::iterator iter;
	// iterate through the map
	for (iter = _orderbook_map.begin(); iter != _orderbook_map.end(); ++iter) {
		OrderBook& orderbook = iter->second;
		// if the sell side of the orderbook is not empty, add the symbol to the vector
		bool flag = (!orderbook.isSellEmpty()) || (!orderbook.isBuyEmpty());
		if (flag == true) {
			tradable.push_back(iter->first);
		}
	}

	return tradable;

}

OrderBook* Exchange::find_orderbook(const string symbol){
	// try to find the orderbook
	OrderBooks::iterator it;
	it = _orderbook_map.find(symbol);
	// if we can find it, return the pointer to the second value(OrderBook*)
	if (it != _orderbook_map.end()) {
		return &(it->second);
	}
	// else, return nullptr
	else {
		return nullptr;
	}
}

void Exchange::process_order(const string & msg_str)
{
	try {
		Order order(msg_str);
		// try creating the order


		// if the order is not valid, do something and abort
		if (!order.isValid) {
			cerr << "Warning: order not valid, maybe missing fields" << endl;
			return;
		}

		string symbol = order.getSymbol();
		// now the order must be valid, check if there is an orderbook
		OrderBook* p_orderbook = find_orderbook(symbol);

		// if we cannot find the orderbook, create a new orderbook with the order in it
		if (p_orderbook == nullptr) {
			OrderBook new_orderbook(symbol);
			new_orderbook.insert(order);
			_orderbook_map.insert(pair<string, OrderBook>(symbol, new_orderbook));
			// because the orderbook only has a single order now, we do not need to match
		}
		// else, just add the order to the orderbook
		else {
			p_orderbook->insert(order);
			// try to match the orders with this symbol
			// TODO: find better match mechanism
			match(symbol);
			process_transactions();
		}
	}
	catch (FIX::Exception ex) {
		cerr << "! ERROR: " << ex.what() << endl;
	}
	
}


