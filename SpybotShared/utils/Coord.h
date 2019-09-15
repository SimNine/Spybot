#pragma once

struct Coord {
	int x;
	int y;

	friend bool operator==(Coord a, Coord b) {
		if (a.x == b.x && a.y == b.y)
			return true;
		else 
			return false;
	}

	friend bool operator!=(Coord a, Coord b) {
		if (a.x != b.x || a.y != b.y)
			return true;
		else
			return false;
	}

	friend Coord operator+(Coord a, Coord b) {
		return { a.x + b.x, a.y + b.y };
	}

	friend Coord operator-(Coord a, Coord b) {
		return { a.x - b.x, a.y - b.y };
	}
};

#define NULLCOORD Coord{-1, -1}