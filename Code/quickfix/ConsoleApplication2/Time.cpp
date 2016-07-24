#include "Time.h"

#include <string>
using namespace std;

void Time::setCurrent()
{
	utctimestamp.setCurrent();
}

string Time::getString()
{
	return FIX::UtcTimeStampConvertor::convert(utctimestamp, true);
}

FIX::UtcTimeStamp Time::getUtcTimeStamp()
{
	return utctimestamp;
}
