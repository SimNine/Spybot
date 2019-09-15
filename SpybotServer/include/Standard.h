#ifndef STANDARD_H
#define STANDARD_H

#include <mutex>
#include <string>
#include <thread>
#include <stdlib.h>
#include <stdio.h>
#include <cmath>
#include <fstream>
#include <iostream>
#undef UNICODE

#define WIN32_LEAN_AND_MEAN
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment (lib, "Ws2_32.lib")
#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "27015"

#include "Enums.h"
#include "Coord.h"
#include "Pair.h"
#include "Entry.h"

#endif