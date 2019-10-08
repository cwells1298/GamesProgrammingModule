#include "Logging.h"

char* getTime(void)
{
	//Get the current time
	time_t currentTime = std::time(0);

	//Get time info from current time
	struct tm* info = localtime(&currentTime);

	char* returnString = new char[32]();

	strftime(returnString, 32, "%d/%m/%y %T", info);

	return returnString;
}

