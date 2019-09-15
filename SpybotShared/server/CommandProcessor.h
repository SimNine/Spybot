#pragma once

#include "Standard.h"

void processCommandLoop();
void processCommandResponse(std::string cmd, int clientID);
void processCommand(std::string cmd, int clientID);