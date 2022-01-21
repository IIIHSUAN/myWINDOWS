#include "Canvas.h"

#include "Window/Window.h"

void Canvas::flush(wchar_t newChar)
{
	std::wstring flushStr(size.width, newChar ? newChar : flushChar);
	for (int i = 0; i < size.height; i++)
		canvas.replace(
			index(i, 0), size.width,
			flushStr
		);

	frame();
}

void Canvas::line(int X, int Y, const wchar_t * str, const int len)  // draw in one line
{
	if (Y > MY_WINDOW_HEIGHT || Y < 0)
		return;

	static int x, w;
	x = X < 0 ? 0 : X;
	w = X + len > MY_WINDOW_WIDTH - isFrame ? MY_WINDOW_WIDTH - X - isFrame : len;

	canvas.replace(index(Y, x), w, str, 0, w);
}

void Canvas::line(int X, int Y, const int showLen, const wchar_t * str, const int len)  // draw in one line
{
	if (Y > MY_WINDOW_HEIGHT || Y < 0)
		return;

	static int x, w;
	x = X < 0 ? 0 : X;
	w = X + len > MY_WINDOW_WIDTH - isFrame ? MY_WINDOW_WIDTH - X - isFrame : len;
	canvas.replace(index(Y, x), showLen, str, 0, w);
}

void Canvas::line(int X, int Y, std::wstring s)  // draw in one line
{
	if (Y > MY_WINDOW_HEIGHT || Y < 0)
		return;

	static int x, w;
	x = X < 0 ? 0 : X;
	w = X + s.length() > MY_WINDOW_WIDTH - isFrame ? MY_WINDOW_WIDTH - X - isFrame : int(s.length());
	canvas.replace(index(Y, x), w, s, 0, w);
}

void Canvas::lineCenter(int X, int Y, std::wstring& str)
{
	line(X - int(str.length() / 2), Y, str.c_str(), int(str.length()));
}

void Canvas::renderWith(Canvas & front)
{
	static int x, y, w, h, mx, my;
	x = front.pos.x, y = front.pos.y;
	w = min(front.size.width, size.width - x - 1), h = min(front.size.height, size.height - y - 1);
	mx = min(0, x - isFrame), my = min(0, y - isFrame);  // if in negative plane

	if (w + mx < 0)
		return;

	for (int i = -my; i < h && w>0; i++)
		canvas.replace(index(i + y, x) - mx, w + mx, front.getCanvas(), i*MY_WINDOW_WIDTH - mx, w + mx);
}

void Canvas::renderWindow(Window& window)  // with custom size
{
	static int x, y, w, h, mx, my;

	x = window.getX(), y = window.getY();
	w = min(window.getSize().width, MY_WINDOW_WIDTH - x), h = min(window.getSize().height, MY_WINDOW_HEIGHT - y);
	mx = min(0, x), my = min(0, y);

	for (int i = -my; i < h && w>0; i++)
		canvas.replace(index(i + y, x) - mx, w + mx, window.getCanvas().canvas, i*MY_WINDOW_WIDTH - mx, w + mx);
}
void Canvas::setCharImage(CharImage & src)
{
	static int x, y, w, h;
	x = x < 0 ? 0 : (x > size.width ? size.width : x);
	y = y < 0 ? 0 : (y > size.height ? size.height : y);

	w = x + src.size.width > size.width ? size.width - x : src.size.width;
	h = y + src.size.height > size.height ? size.height - y : src.size.height;

	for (int i = 0; i < h; i++)
		canvas.replace(
			index(i + y, x), w - 1,
			src.data,
			i*src.size.width - i, w - 1
		);
}

void Canvas::setPos4h()  // to-do: min(), max(), clamp(), calc()
{
	int value =
		pos4.hAttr.unit == vw ? int(pos4.hAttr.value * parentSize.width / 100.0f) :
		pos4.hAttr.unit == vh ? int(pos4.hAttr.value * parentSize.height / 100.0f) :
		pos4.hAttr.value;

	if (pos4.hAttr.dir == Dir::right)
		pos.x = (pos4.hAttr.position == Position::relative ? parentSize.width - size.width : originParentSize.width - size.width) - value;
	else
		pos.x = (pos4.hAttr.position == Position::relative ? 0 : originParentPos.x - parentPos.x) + value;
}
void Canvas::setPos4v()
{
	int value =
		pos4.vAttr.unit == vw ? int(pos4.vAttr.value * parentSize.width / 100.0f) :
		pos4.vAttr.unit == vh ? int(pos4.vAttr.value * parentSize.height / 100.0f) :
		pos4.vAttr.value;

	if (pos4.vAttr.dir == Dir::bottom)
		pos.y = (pos4.vAttr.position == Position::relative ? parentSize.height - size.height : originParentSize.height - size.height) - value;
	else
		pos.y = (pos4.vAttr.position == Position::relative ? 0 : originParentPos.y - parentPos.y) + value;
}

void Canvas::frame()
{
	if(isFrame)
		for (int i = 0; i < size.height; i++)
		{
			if (i == 0 || i == size.height - 1)  // top & bottom frame
				canvas.replace(index(i, 0), size.width, size.width, L'\u2550');
			canvas.at(index(i, 0)) = (i == 0 ? L'\u2554' : (i == size.height - 1) ? L'\u255A' : L'\u2551');  // left
			canvas.at(index(i, size.width - 1)) = (i == 0 ? L'\u2557' : (i == size.height - 1) ? L'\u255D' : L'\u2551');  // right
		}
}
