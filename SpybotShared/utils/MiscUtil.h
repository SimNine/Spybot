#ifndef MISCUTIL_H
#define MISCUTIL_H

#include <string>
#include <mutex>

#include "Coord.h"

void deserializeInt(char* buf, int* i);

void serializeInt(char* buf, int i);

std::string to_string(int);

int dist(Coord, Coord);

int tokenize(char** pointer, const char* string, char delimiter);

void destroyTokens(char** tokens);

extern int currRand;
extern std::mutex randMutex;
int randInt();

#endif // MISCUTIL_H