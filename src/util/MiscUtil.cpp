#include <cstdio>
#include <string>
#include <cassert>

#include "MiscUtil.h"

std::string to_string(int i)
{
    int length = snprintf(NULL, 0, "%d", i);
    assert(length >= 0);
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
