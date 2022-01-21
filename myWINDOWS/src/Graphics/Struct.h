#pragma once

#define min(a,b) (((a) < (b)) ? (a) : (b))
#define max(a,b) (((a) > (b)) ? (a) : (b))

/* BufferRender *********************************************************/

#define MY_WINDOW_HEIGHT 29
#define MY_WINDOW_WIDTH 100
#define MY_WINDOW_PIXELS MY_WINDOW_HEIGHT * MY_WINDOW_WIDTH

static int index(int Y, int X)
{
	return Y * MY_WINDOW_WIDTH + X;
}

/* css Attr *********************************************************/

enum class TextAlign { left, right, center };

/* Pos4 *********************************************************/

enum Position { absolute, relative };
enum class Dir { top, right, bottom, left };
enum Unit { px, vw, vh };

struct PosHAttr
{
	Dir dir;
	int value;
	Unit unit;
	Position position;
};
struct Left :public PosHAttr
{
	Left(int value, Unit unit = px, Position position = absolute) : PosHAttr({ Dir::left,value,unit,position }) {}
};
struct Right :public PosHAttr
{
	Right(int value, Unit unit = px, Position position = absolute) : PosHAttr({ Dir::right,value,unit,position }) {}
};

struct PosVAttr
{
	Dir dir;
	int value;
	Unit unit;
	Position position;
};
struct Top :public PosVAttr
{
	Top(int value, Unit unit = px, Position position = absolute) : PosVAttr({ Dir::top,value,unit,position }) {}
};
struct Bottom :public PosVAttr
{
	Bottom(int value, Unit unit = px, Position position = absolute) : PosVAttr({ Dir::bottom,value,unit,position }) {}
};

struct Pos4
{
	PosHAttr hAttr;
	PosVAttr vAttr;
};

/* Pos, Size (origin) *********************************************************/

struct Pos
{
	int x, y;

	inline bool operator!=(const int& i) { return !(x == i && y == i); }
	inline bool operator!=(Pos& pos) { return !(x == pos.x&&y == pos.y); }
	inline bool operator==(Pos&& pos) { return x == pos.x&&y == pos.y; }
	inline bool operator==(Pos& pos) { return x == pos.x&&y == pos.y; }
	inline bool operator==(const int& i) { return x == i&&y == i; }
	inline Pos operator++(int) { x += 1, y += 1; return Pos({ x - 1, y - 1 }); }
	inline Pos operator--(int) { x -= 1, y -= 1; return Pos({ x + 1, y + 1 }); }
	inline void operator()(int _x, int _y) { x = _x, y = _y; }
	inline void operator+=(Pos& pos) { x += pos.x, y += pos.y; }
	inline void operator-=(Pos& pos) { x -= pos.x, y -= pos.y; }
};

struct Size
{
	int width, height;

	inline bool operator!=(const int& i) { return !(width == i && height == i); }
	inline bool operator!=(Size& pos) { return !(width == pos.width&&height == pos.height); }
	inline bool operator==(Size&& pos) { return width == pos.width&&height == pos.height; }
	inline bool operator==(Size& pos) { return width == pos.width&&height == pos.height; }
	inline bool operator==(const int& i) { return width == i && height == i; }
	inline Size operator++(int) { width += 1, height += 1; return Size({ width - 1, height - 1 }); }
	inline Size operator--(int) { width -= 1, height -= 1; return Size({ width + 1, height + 1 }); }
	inline void operator()(int _width, int _height) { width = _width, height = _height; }
	inline void operator+=(Size& pos) { width += pos.width, height += pos.height; }
	inline void operator-=(Size& pos) { width -= pos.width, height -= pos.height; }
};