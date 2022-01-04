#pragma once

#define min(a,b) (((a) < (b)) ? (a) : (b))
#define max(a,b) (((a) > (b)) ? (a) : (b))

#define MY_WINDOW_HEIGHT 28
#define MY_WINDOW_WIDTH 110
#define MY_WINDOW_PIXELS MY_WINDOW_HEIGHT * MY_WINDOW_WIDTH

struct Pos
{
	int x, y;

	inline bool operator==(Pos&& pos) { return x == pos.x&&y == pos.y; }
	inline bool operator==(Pos& pos) { return x == pos.x&&y == pos.y; }
	inline bool operator==(const int& i) { return x == i&&y == i; }
	inline Pos operator++(int) { x += 1, y += 1; return Pos({ x - 1, y - 1 }); }
	inline Pos operator--(int) { x -= 1, y -= 1; return Pos({ x + 1, y + 1 }); }
	inline void operator()(int _x, int _y) { x = _x, y = _y; }
};

struct Size
{
	int width, height;
};

static int index(int Y, int X)
{
	return Y* MY_WINDOW_WIDTH+ X;
}