// Basic time class, utilizing FIX::UtcTimeStamp

#ifndef _EXCHANGE_TIME_H
#define _EXCHANGE_TIME_H

#include <string>

#include "quickfix\Message.h"

using namespace std;

class Time {
public:
	// constuctor: set the time
	Time() {
		utctimestamp.setCurrent();
	}

	void setCurrent();						// set the current time (UTC time)
	string getString();						// get a string representing the current time
	FIX::UtcTimeStamp getUtcTimeStamp();	// get the UtcTimeStamp object

private:
	FIX::UtcTimeStamp utctimestamp;
};




#endif