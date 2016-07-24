/* Created by Yang Liu, July 2016.
* This header file defines a transaction class
* which stores a variety of properties of a transaction.
*
*/

#ifndef _TRANSACTION_H
#define _TRANSACTION_H

#include <string>
#include <iostream>
#include <vector>

#include "error.h"
#include "Order.h"
#include "Time.h"

using namespace std;

class Transaction {
public:
	// ----------- operator overloading ------------
	friend ostream& operator<<(ostream& ostream, Transaction& transaction);

	// ----------- constructors ------------
	Transaction() { valid = false; };		// DANGEROUS! So we set valid to false.		
	Transaction(Order& sell_order, Order& buy_order);

	// ----------- member functions -------------
	string getSymbol() { 
		if (!valid) error("transaction not valid");
		return symbol; 
	}
	double getPrice() { 
		if (!valid) error("transaction not valid");
		return price;
	}
	long getQuantity() { 
		if (!valid) error("transaction not valid");
		return quantity;
	}
	string getSeller() { 
		if (!valid) error("transaction not valid");
		return seller;
	}
	string getBuyer() { 
		if (!valid) error("transaction not valid");
		return buyer;
	}
	bool isValid() { 
		return valid;
	}
	
	void validate();		// validate and set the private fields


	// ----------- variables ------------
	// store the two orders for future references
	Order sellOrder;
	Order buyOrder;


private:
	// basic properties of this transaction
	string symbol;

	double price;
	long quantity;

	string seller;
	string buyer;

	bool valid;				// a transaction is not valid if is constructed from default constrcutor
							// if passes validate(), then the transaction becomes valid

	Time _time;				// time object that stores the utc time when the transaction was filed
};
//
//class BadTransaction : public Transaction {
//	// override isSuccessful():
//
//};


#endif
