#pragma once

#include <string>
#include <list>

#include "Struct.h"

class Window;

#define PUSH_CHAR_IMAGE(canvas, ci_ptr, ci_entity) ci_ptr.reset(new ci_entity), canvas.pushCharImage(ci_ptr)
#define TRANSPARENT_WCHAR L' '
#define WHITESPACE_WCHAR L'\x2000'

struct CharImage
{
	const wchar_t* data;
	Pos4 pos4;
	Size size;
	bool isVisible = true;

	CharImage(const wchar_t* data, Pos4 pos4, Size size) :data(data), pos4(pos4), size(size) {}
};

class Canvas
{
public:
	Canvas() {}
	Canvas(Pos pos, Size size, bool isFrame = false, wchar_t flushChar = L' ')  // for Window constructor, on absolute zero
		: pos(pos), originPos(pos), size(size), originSize(size), isFrame(isFrame),	canvas(std::wstring(MY_WINDOW_PIXELS, flushChar)), flushChar(flushChar) {}

	Canvas(Pos4 pos4, Size2 size2, Canvas& parent, bool isFrame = false, wchar_t flushChar = L' ')  // on parent zero
		: pos4(pos4), size2(size2), isFrame(isFrame), canvas(std::wstring(MY_WINDOW_PIXELS, flushChar)), flushChar(flushChar) {
		setSize2(size2, parent), originSize = size;
		setPos4(pos4, parent), originPos = pos;
	}

	void flush(wchar_t newChar = 0);
	void frame();

	void line(int X, int Y, const int showLen, const wchar_t * str, const int len, bool isWhitespace = true);
	void line(int X, int Y, const wchar_t * str, const int len, bool isWhitespace = true);
	void line(int X, int Y, std::wstring s, bool isWhitespace = true);
	void lineCenter(int X, int Y, std::wstring& str, bool isWhitespace = true);
	inline void canvasCenterLine(std::wstring& str, bool isWhitespace = true) { lineCenter(size.width / 2, size.height / 2, str, isWhitespace); }

	void renderWith(Canvas& frontl);
	void renderCharImage(CharImage& source, bool isWhitespace);
	void renderWindow(Window& window);

	int convertPos4h(const Pos4& pos4, Canvas& parent);
	int convertPos4v(const Pos4& pos4, Canvas& parent);
	int convertSize2width(const Size2& size2, Canvas& parent);
	int convertSize2height(const Size2& size2, Canvas& parent);

	inline void setPos(Pos _pos) { pos = _pos; }  // for window use only, others use setPos4 for pos config
	inline void setSize(Size _size) { size = _size; }  // for window use only, others use setSize2 for size config

	inline void setPos4(Pos4& _pos4, Canvas& parent) { pos4 = _pos4; setPos4h(parent), setPos4v(parent); }
	inline void setPos4(Pos4&& _pos4, Canvas& parent) { pos4 = _pos4; setPos4h(parent), setPos4v(parent); }
	inline void setPos4hValue(int value, Canvas& parent) { pos4.hAttr.value = value; setPos4h(parent); }
	inline void setPos4vValue(int value, Canvas& parent) { pos4.vAttr.value = value; setPos4v(parent); }
	inline void setSize2(Size2& _size2, Canvas& parent) { size2 = _size2; setSize2width(parent), setSize2height(parent); }
	inline void setSize2(Size2&& _size2, Canvas& parent) { size2 = _size2; setSize2width(parent), setSize2height(parent); }
	inline void setSize2widthValue(int value, Canvas& parent) { size2.width.value = value; setSize2width(parent); }
	inline void setSize2heightValue(int value, Canvas& parent) { size2.height.value = value; setSize2height(parent); }

	inline void setIsFrame(bool b) { isFrame = b; }

	inline Pos4& getPos4() { return pos4; }
	inline Pos& getPos() { return pos; }
	inline Size2& getSize2() { return size2; }
	inline Size& getSize() { return size; }
	inline const Pos& getOriginPos() { return pos; }
	inline const Size& getOriginSize() { return size; }
	inline bool& getIsFrame() { return isFrame; }
	inline std::wstring& getCanvas() { return canvas; }
	inline const wchar_t* getConstCanvas() { return canvas.c_str(); }

private:
	wchar_t flushChar;
	std::wstring canvas;
	bool isFrame;
	Pos4 pos4;
	Pos pos, originPos;
	Size2 size2;
	Size size, originSize;
	
	template<class T> void _renderLine(const int start, const int& width, T& src, const int srcStart, bool isWhitespace = false);
	void _renderLine(const int start, const int& width, const wchar_t src);

	inline void setPos4h(Canvas& parent) { pos.x = convertPos4h(pos4, parent); }
	inline void setPos4v(Canvas& parent) { pos.y = convertPos4v(pos4, parent); }
	inline void setSize2width(Canvas& parent) { size.width = convertSize2width(size2, parent); }
	inline void setSize2height(Canvas& parent) { size.height = convertSize2height(size2, parent); }
};

template<class T>
inline void Canvas::_renderLine(const int start, const int& width, T& src, const int srcStart, bool isWhitespace)
{
	for (int i = 0; i < width; i++)
		if (isWhitespace && src[srcStart + i] == TRANSPARENT_WCHAR)
			canvas[start + i] = WHITESPACE_WCHAR;
		else if (src[srcStart + i] != TRANSPARENT_WCHAR)
			canvas[start + i] = src[srcStart + i];
}

inline void Canvas::_renderLine(const int start, const int & width, const wchar_t src)
{
	for (int i = 0; i < width; i++)
		canvas[start + i] = src;
}