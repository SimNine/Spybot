#include "Standard.h"
#include "NameGenerator.h"

std::string generateName() {
	const int numPrefixes = 6;
	std::string prefixes[numPrefixes];
	prefixes[0] = "The";
	prefixes[1] = "123";
	prefixes[2] = "XxX";
	prefixes[3] = "Power";
	prefixes[4] = "";
	prefixes[5] = "~";

	const int numMainNames = 7;
	std::string mainName[numMainNames];
	mainName[0] = "TERMINATOR";
	mainName[1] = "Fearless";
	mainName[2] = "BoBBy";
	mainName[3] = "YEEEEEE";
	mainName[4] = "sixflags";
	mainName[5] = "HRNG";
	mainName[6] = "2048";

	const int numSuffixes = 6;
	std::string suffixes[numSuffixes];
	suffixes[0] = "god";
	suffixes[1] = "321";
	suffixes[2] = "XxX";
	suffixes[3] = "bot";
	suffixes[4] = "";
	suffixes[5] = "~";

	std::string ret = "";
	ret += prefixes[rand() % numPrefixes];
	ret += mainName[rand() % numMainNames];
	ret += suffixes[rand() % numSuffixes];
	return ret;
}