
#include "Transaction.h"
#include "error.h"

using namespace std;

Transaction::Transaction(Order& sell_order, Order& buy_order)
	:sellOrder(sell_order), buyOrder(buy_order)
{
	// validate the symbol, price, quantity of the two orders,
	// if valid, store the symbol, price, quantity
	if (sellOrder.getSymbol() != buyOrder.getSymbol()) {
		error("Transaction: constructor: symbol not match");
	}
	symbol = sellOrder.getSymbol();

	if (sellOrder.getLastExecutedQuantity() != buyOrder.getLastExecutedQuantity()) {
		error("Transaction: constructor: lastExecutedQuantity not match");
	}
	quantity = sellOrder.getLastExecutedQuantity();

	if (sellOrder.getLastExecutedPrice() != buyOrder.getLastExecutedPrice()) {
		error("Transaction: constructor: lastExecutedPrice not match");
	}
	price = sellOrder.getLastExecutedPrice();

	// store the buyer and seller
	seller = sellOrder.getOwner();
	buyer = buyOrder.getOwner();
	
	valid = true;
}

void Transaction::validate() {
	if (sellOrder.getSymbol() != buyOrder.getSymbol()) {
		error("Transaction: constructor: symbol not match");
	}
	symbol = sellOrder.getSymbol();

	if (sellOrder.getLastExecutedQuantity() != buyOrder.getLastExecutedQuantity()) {
		error("Transaction: constructor: lastExecutedQuantity not match");
	}
	quantity = sellOrder.getLastExecutedQuantity();

	if (sellOrder.getLastExecutedPrice() != buyOrder.getLastExecutedPrice()) {
		error("Transaction: constructor: lastExecutedPrice not match");
	}
	price = sellOrder.getLastExecutedPrice();

	// store the buyer and seller
	seller = sellOrder.getOwner();
	buyer = buyOrder.getOwner();

	valid = true;
}