#include "MiscUtil.h"

void deserializeInt(char* buf, int* i)
{
	char* iptr = reinterpret_cast<char*>(i);
	iptr[0] = buf[0];
	iptr[1] = buf[1];
	iptr[2] = buf[2];
	iptr[3] = buf[3];
}

void serializeInt(char* buf, int i)
{
	char* chptr = reinterpret_cast<char*>(&i);
	buf[0] = chptr[0];
	buf[1] = chptr[1];
	buf[2] = chptr[2];
	buf[3] = chptr[3];
}

std::string to_string(int i)
{
    int length = snprintf(NULL, 0, "%d", i);
    //assert(length >= 0);
    char* buf = new char[length+1];
    snprintf(buf, length+1, "%d", i);
    std::string str(buf);
    delete[] buf;
    return str;
}

// gets the non-square-rooted distance from one point to another
int dist(Coord a, Coord b)
{
    return ((a.x-b.x)*(a.x-b.x) + (a.y-b.y)*(a.y-b.y));
}
