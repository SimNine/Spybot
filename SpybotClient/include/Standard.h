#pragma once

#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <cstdio>
#include <assert.h>
#include <fstream>
#include <cmath>
#include <thread>
#include <mutex>

#include <SDL.h>
#include <SDL_Mixer.h>
#include <SDL_Image.h>
#include <SDL_TTF.h>

#define WIN32_LEAN_AND_MEAN
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment (lib, "Ws2_32.lib")
#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "27015"

#include "Enums.h"
#include "Coord.h"
#include "Data.h"