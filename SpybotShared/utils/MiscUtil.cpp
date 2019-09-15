#include "MiscUtil.h"

#include <mutex>

// deserializes an integer from a buffer into an int pointer
void deserializeInt(char* buf, int* i) {
	char* iptr = reinterpret_cast<char*>(i);
	iptr[0] = buf[0];
	iptr[1] = buf[1];
	iptr[2] = buf[2];
	iptr[3] = buf[3];
}

// serializes an integer into a buffer
void serializeInt(char* buf, int i) {
	char* chptr = reinterpret_cast<char*>(&i);
	buf[0] = chptr[0];
	buf[1] = chptr[1];
	buf[2] = chptr[2];
	buf[3] = chptr[3];
}

// gets the non-square-rooted distance from one point to another
int dist(Coord a, Coord b) {
	return ((a.x - b.x)*(a.x - b.x) + (a.y - b.y)*(a.y - b.y));
}

// tokenizes a string into a pointer to a string array
// returns the number of tokens created
int tokenize(char** tokens, const char* string, char delimiter) {
	char buffer[1024]; // array of characters to make a string of
	int currToken = 0;
	int currPos = 0;

	for (int i = 0; i < strlen(string); i++) {
		if ((string[i] == delimiter || string[i] == '\n') && currPos == 0) {
			continue;
		} else if (string[i] == delimiter || string[i] == '\n' || i == strlen(string)) {
			buffer[currPos] = '\0';
			tokens[currToken] = (char*)calloc(currPos, (sizeof(char) * strlen(buffer)) + 1);

			for (int j = 0; j < strlen(buffer); j++) {
				tokens[currToken][j] = buffer[j];
			}

			currToken++;
			currPos = 0;
		} else {
			buffer[currPos] = string[i];
			currPos++;
		}
	}

	// end the array
	if (currPos == 0) {
		tokens[currToken] = NULL;
		return currToken;
	} else {
		tokens[currToken + 1] = NULL;
		return currToken + 1;
	}
}

// destroys an array of tokens
void destroyTokens(char** tokens) {
	for (int i = 0; ; i++) {
		if (tokens[i] == NULL) {
			break;
		} else {
			free(tokens[i]);
			tokens[i] = NULL;
		}
	}
}

// gets a random integer
int currRand = 1;
std::mutex randMutex;
int randInt() {
	randMutex.lock();
	int temp = currRand;
	currRand++;
	randMutex.unlock();
	return temp;
}