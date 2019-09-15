#ifndef MISCUTIL_H
#define MISCUTIL_H

#include <string>

#include "Coord.h"

void deserializeInt(char* buf, int* i);

void serializeInt(char* buf, int i);

std::string to_string(int);

int dist(Coord, Coord);

#endif // MISCUTIL_H
