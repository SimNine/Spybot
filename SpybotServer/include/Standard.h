#pragma once

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

#include <stdint.h>

#define SDL_MAX_UINT8   ((Uint8)0xFF)           /* 255 */
#define SDL_MIN_UINT8   ((Uint8)0x00)           /* 0 */
typedef uint8_t Uint8;

typedef struct SDL_Color {
	Uint8 r;
	Uint8 g;
	Uint8 b;
	Uint8 a;
} SDL_Color;