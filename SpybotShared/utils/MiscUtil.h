#pragma once

#include <string>
#include <mutex>

#include "Coord.h"

void deserializeInt(char* buf, int* i);

void serializeInt(char* buf, int i);

int dist(Coord, Coord);

int tokenize(char** pointer, const char* string, char delimiter);

void destroyTokens(char** tokens);

extern int currRand;
extern std::mutex randMutex;
int randInt();