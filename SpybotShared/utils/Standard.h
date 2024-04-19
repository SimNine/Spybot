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

#include "SDL.h"

#define WIN32_LEAN_AND_MEAN
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment (lib, "Ws2_32.lib")
#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "27015"

#include "Logger.h"
#include "Enums.h"
#include "Coord.h"
#include "Pair.h"
#include "Entry.h"

#include <stdint.h>

#define sdl_max_uint8   ((uint8)0xff)           /* 255 */
#define sdl_min_uint8   ((uint8)0x00)           /* 0 */
typedef uint8_t uint8;

typedef struct sdl_color {
	uint8 r;
	uint8 g;
	uint8 b;
	uint8 a;
} sdl_color;