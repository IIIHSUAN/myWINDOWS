#include "Canvas.h"

#include "Window/Window.h"

void Canvas::flush(wchar_t newChar)
{
	for (int i = 0; i < size.height; i++)
		_renderLine(index(i, 0), size.width, newChar ? newChar : flushChar);
		/*canvas.replace(
			index(i, 0), size.width,
			flushStr
		);*/

	frame();
}

void Canvas::line(int X, int Y, const wchar_t * str, const int len, bool isWhitespace)  // draw in one line
{
	if (Y > MY_WINDOW_HEIGHT || Y < 0)
		return;

	static int x, w;
	x = X < 0 ? 0 : X;
	w = X + len > MY_WINDOW_WIDTH - isFrame ? MY_WINDOW_WIDTH - X - isFrame : len;

	_renderLine(index(Y, x), w, str, 0, isWhitespace);  //canvas.replace(index(Y, x), w, str, 0, w);
}
void Canvas::line(int X, int Y, const int showLen, const wchar_t * str, const int len, bool isWhitespace)  // draw in one line
{
	if (Y > MY_WINDOW_HEIGHT || Y < 0)
		return;

	static int x, w;
	x = X < 0 ? 0 : X;
	w = X + len > MY_WINDOW_WIDTH - isFrame ? MY_WINDOW_WIDTH - X - isFrame : len;
	_renderLine(index(Y, x), min(showLen, w), str, 0, isWhitespace);  //canvas.replace(index(Y, x), showLen, str, 0, w);
}
void Canvas::line(int X, int Y, std::wstring s, bool isWhitespace)  // draw in one line
{
	if (Y > MY_WINDOW_HEIGHT || Y < 0)
		return;

	static int x, w;
	x = X < 0 ? 0 : X;
	w = X + s.length() > MY_WINDOW_WIDTH - isFrame ? MY_WINDOW_WIDTH - X - isFrame : int(s.length());
	_renderLine(index(Y, x), w, s, 0, isWhitespace);  //canvas.replace(index(Y, x), w, s, 0, w);
}
void Canvas::lineCenter(int X, int Y, std::wstring& str, bool isWhitespace)
{
	line(X - int(str.length() / 2), Y, str.c_str(), int(str.length()), isWhitespace);
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
		_renderLine(index(i + y, x) - mx, w + mx, front.getCanvas(), i*MY_WINDOW_WIDTH - mx);  //canvas.replace(index(i + y, x) - mx, w + mx, front.getCanvas(), i*MY_WINDOW_WIDTH - mx, w + mx);
}
void Canvas::renderCharImage(CharImage & src, bool isWhitespace)
{
	static int x, y, w, h;
	x = x < 0 ? 0 : (x > size.width ? size.width : x);
	y = y < 0 ? 0 : (y > size.height ? size.height : y);

	w = x + src.size.width > size.width ? size.width - x : src.size.width;
	h = y + src.size.height > size.height ? size.height - y : src.size.height;

	for (int i = 0; i < h; i++)
		_renderLine(index(i + y, x), w, src.data, i*(src.size.width - 1), isWhitespace);
		/*canvas.replace(
			index(i + y, x), w,
			src.data,
			i*(src.size.width - 1), w
		);*/

	frame();
}
void Canvas::renderWindow(Window& window)  // with custom size
{
	static int x, y, w, h, mx, my;

	x = window.getX(), y = window.getY();
	w = min(window.getSize().width, MY_WINDOW_WIDTH - x), h = min(window.getSize().height, MY_WINDOW_HEIGHT - y);
	mx = min(0, x), my = min(0, y);

	for (int i = -my; i < h && w>0; i++)
		_renderLine(index(i + y, x) - mx, w + mx, window.getCanvas().canvas, i*MY_WINDOW_WIDTH - mx);  //canvas.replace(index(i + y, x) - mx, w + mx, window.getCanvas().canvas, i*MY_WINDOW_WIDTH - mx, w + mx);
}

int Canvas::convertPos4h(const Pos4 & pos4, Canvas& parent)
{
	int value =
		pos4.hAttr.unit == vw ? int(pos4.hAttr.value * parent.size.width / 100.0f) :
		pos4.hAttr.unit == vh ? int(pos4.hAttr.value * parent.size.height / 100.0f) :
		pos4.hAttr.value;

	if (pos4.hAttr.dir == Dir::right)
		return (pos4.hAttr.position == Position::relative ? parent.size.width : parent.originPos.x - parent.pos.x + parent.originSize.width)
		- size.width - value;
	else
		return (pos4.hAttr.position == Position::relative ? 0 : parent.originPos.x - parent.pos.x) + value;
}
int Canvas::convertPos4v(const Pos4 & pos4, Canvas& parent)
{
	int value =
		pos4.vAttr.unit == vw ? int(pos4.vAttr.value * parent.size.width / 100.0f) :
		pos4.vAttr.unit == vh ? int(pos4.vAttr.value * parent.size.height / 100.0f) :
		pos4.vAttr.value;

	if (pos4.vAttr.dir == Dir::bottom)
		return (pos4.vAttr.position == Position::relative ? parent.size.height : parent.originPos.y - parent.pos.y + parent.originSize.height) 
		- size.height - value;
	else
		return (pos4.vAttr.position == Position::relative ? 0 : parent.originPos.y - parent.pos.y) + value;
}

int Canvas::convertSize2width(const Size2 & size2, Canvas& parent)
{
	return size2.width.unit == vw ? int(size2.width.value * parent.size.width / 100.0f) :
		size2.width.unit == vh ? int(size2.width.value * parent.size.height / 100.0f) :
		size2.width.value;
}
int Canvas::convertSize2height(const Size2 & size2, Canvas& parent)
{
	return size2.height.unit == vw ? int(size2.height.value * parent.size.height / 100.0f) :
		size2.height.unit == vh ? int(size2.height.value * parent.size.height / 100.0f) :
		size2.height.value;
}

void Canvas::frame()
{
	if(isFrame)
		for (int i = 0; i < size.height; i++)
		{
			if (i == 0 || i == size.height - 1)  // top & bottom frame
				_renderLine(index(i, 0), size.width, L'\u2550');  //canvas.replace(index(i, 0), size.width, size.width, L'\u2550');
			canvas.at(index(i, 0)) = (i == 0 ? L'\u2554' : (i == size.height - 1) ? L'\u255A' : L'\u2551');  // left
			canvas.at(index(i, size.width - 1)) = (i == 0 ? L'\u2557' : (i == size.height - 1) ? L'\u255D' : L'\u2551');  // right
		}
}
