#pragma once

#include <string>

void logInit(std::string fileName, bool append);

void log(std::string str);

std::string to_string(int value);

std::string to_string(float value, int numSigFigs);