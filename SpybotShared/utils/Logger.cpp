#include "MiscUtil.h"

#include <iostream>
#include <fstream>

static std::ofstream logObj;

void logInit(std::string fileName, bool append) {
	if (append)
		logObj.open(fileName, std::ios::out | std::ios::app);
	else
		logObj.open(fileName, std::ios::out | std::ios::trunc);
}

void log(std::string str) {
	std::cout << str;

	logObj << str;
	logObj.flush();
}

// converts an int to a string
std::string to_string(int i) {
	int length = snprintf(NULL, 0, "%d", i);
	//assert(length >= 0);
	char* buf = new char[length + 1];
	snprintf(buf, length + 1, "%d", i);
	std::string str(buf);
	delete[] buf;
	return str;
}