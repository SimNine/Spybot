#ifndef _MAIN_H_
#define _MAIN_H_

#include "Standard.h"

class Server;

extern Server* server;
extern DEBUG debug;

int tokenize(char** pointer, const char* string, char delimiter);
void destroyTokens(char** tokens);

#endif