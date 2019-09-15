#ifndef COORD_H_INCLUDED
#define COORD_H_INCLUDED

struct Coord
{
    int x;
    int y;

    friend bool operator==(Coord a, Coord b)
    {
        if (a.x == b.x && a.y == b.y) return true;
        else return false;
    }

    friend Coord operator+(Coord a, Coord b)
    {
        return {a.x + b.x, a.y + b.y};
    }

    friend Coord operator-(Coord a, Coord b)
    {
        return {a.x - b.x, a.y - b.y};
    }
};

#endif // COORD_H_INCLUDED
