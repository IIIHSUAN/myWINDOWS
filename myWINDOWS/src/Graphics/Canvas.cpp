#include "Canvas.h"

#include "Window/Window.h"

Canvas::Canvas(Pos pos, Size size, bool isFrame, wchar_t flushChar)
	: pos(pos), size(size), isFrame(isFrame), canvas(std::wstring(MY_WINDOW_PIXELS, flushChar)), flushChar(flushChar)
{
	if (isFrame)  // boundaries
		for (int i = 0; i < size.height; i++)
		{
			canvas.replace(index(i, 0), 1, size.width, (i == 0 || i == size.height - 1) ? L'\u2550' : L' ');  // x axis
			canvas.at(index(i, 0)) = (i == 0 ? L'\u2554' : (i == size.height - 1) ? L'\u255A' : L'\u2551');  // left
			canvas.at(index(i, size.width - 1)) = (i == 0 ? L'\u2557' : (i == size.height - 1) ? L'\u255DE' : L'\u2551');  // right
		}
}

void Canvas::flush(wchar_t newChar)
{
	std::wstring flushStr(size.width - isFrame*2, newChar ? newChar : flushChar);
	for (int i = isFrame; i < size.height - isFrame; i++)
		canvas.replace(
			index(i, isFrame), size.width - isFrame*2,
			flushStr
		);
}

void Canvas::line(int X, int Y, const wchar_t * str, const int len)  // draw in one line
{
	if (Y > MY_WINDOW_HEIGHT || Y < 0)
		return;

	static int x, w;
	x = X < 0 ? 0 : X;
	w = X + len > MY_WINDOW_WIDTH ? MY_WINDOW_WIDTH - X : len;
	canvas.replace(index(Y, x), w, str, 0, w);
}

void Canvas::lineCenter(int X, int Y, std::wstring& str)
{
	line(X - str.length() / 2, Y, str.c_str(), str.length());
}

void Canvas::renderWithRel(Canvas & front)
{
	bool isNFrame = !front.isFrame;
	static int x, y, w, h;
	x = front.pos.x, y = front.pos.y, w = min(front.size.width, size.width - x-1), h = min(front.size.height, size.height - y-1);
	static int mx, my; mx = min(0, x), my = min(0, y);

	for (int i = -my + isNFrame; i < h && w>0; i++)
		canvas.replace(index(i + y, x) - mx+ isNFrame, w + mx- isNFrame, front.getCanvas(), i*MY_WINDOW_WIDTH - mx, w + mx - isNFrame);
}

void Canvas::renderWindow(Window& window)  // with custom size
{
	static int x, y, w, h;

	x = window.getX(), y = window.getY(), w = min(window.getWidth(), MY_WINDOW_WIDTH - x), h = min(window.getHeight(), MY_WINDOW_HEIGHT - y);
	static int mx, my; mx = min(0, x), my = min(0, y);

	for (int i = -my; i < h && w>0; i++)
		canvas.replace(index(i + y, x) - mx, w + mx, window.getCanvas().canvas, i*MY_WINDOW_WIDTH - mx, w + mx);
}
void Canvas::setBackground(CharImage & src)
{
	static int w, h;
	w = src.pos.x + src.size.width > size.width ? size.width - src.pos.x : src.size.width;
	h = src.pos.y + src.size.height > size.height ? size.height - src.pos.y : src.size.height;

	for (int i = 0; i < h; i++)
		canvas.replace(
			index(i + src.pos.y, src.pos.x), w - 1,
			src.data,
			i*src.size.width - i, w - 1
		);
}
