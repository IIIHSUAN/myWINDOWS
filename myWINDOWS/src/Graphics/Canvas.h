#pragma once

#include <string>
#include <list>

#include "Struct.h"

class Window;

#define PUSH_CHAR_IMAGE(canvas, ci_ptr, ci_entity) ci_ptr.reset(new ci_entity), canvas.pushCharImage(ci_ptr)

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
		: pos(pos), size(size), isFrame(isFrame), canvas(std::wstring(MY_WINDOW_PIXELS, flushChar)), flushChar(flushChar) {}

	Canvas(Pos4 pos4, Size size, const Pos& parentPos, const Size& parentSize, bool isFrame = false, wchar_t flushChar = L' ')  // on absolute zero
		: pos4(pos4), originParentPos(parentPos), parentPos(parentPos),
		  size(size), originParentSize(parentSize), parentSize(parentSize), 
		isFrame(isFrame), canvas(std::wstring(MY_WINDOW_PIXELS, flushChar)), flushChar(flushChar) {	setPos4(pos4); }

	void flush(wchar_t newChar = 0);
	void frame();

	void line(int X, int Y, const int showLen, const wchar_t * str, const int len);
	void line(int X, int Y, const wchar_t * str, const int len);
	void line(int X, int Y, std::wstring s);
	void lineCenter(int X, int Y, std::wstring& str);
	inline void canvasCenterLine(std::wstring& str) { lineCenter(size.width / 2, size.height / 2, str); }

	void renderWith(Canvas& frontl);
	void renderWindow(Window& window);

	void setCharImage(CharImage& source);
	void setPos4h();
	void setPos4v();
	inline void setParentWidth(int width) { 
		parentSize.width = width; 
		if (pos4.hAttr.position == Position::relative || pos4.hAttr.unit == vw) setPos4h();
		if (pos4.vAttr.unit == vw) setPos4v();
	}
	inline void setParentHeight(int height) { 
		parentSize.height = height; 
		if (pos4.vAttr.position == Position::relative || pos4.vAttr.unit == vh) setPos4v();
		if (pos4.vAttr.unit == vh) setPos4h();
	}
	inline void setWidth(int width) {
		size.width = width; if (pos4.hAttr.position == Position::relative) setPos4h();
	}
	inline void setHeight(int height) { 
		size.height = height; if (pos4.vAttr.position == Position::relative) setPos4v();
	}
	inline void setPos(Pos _pos) { pos = _pos; }
	inline void setPos4(Pos4& _pos4) { pos4 = _pos4; setPos4h(), setPos4v(); }
	inline void setSize(Size _size) { setWidth(_size.width), setHeight(_size.height); }
	inline void setIsFrame(bool b) { isFrame = b; }

	inline Pos4& getPos4() { return pos4; }
	inline Pos& getPos() { return pos; }
	inline Size& getSize() { return size; }
	inline bool& getIsFrame() { return isFrame; }
	inline std::wstring& getCanvas() { return canvas; }
	inline const wchar_t* getConstCanvas() { return canvas.c_str(); }
private:
	wchar_t flushChar;
	std::wstring canvas;
	bool isFrame;
	Pos4 pos4;
	Pos pos, originParentPos, parentPos;
	Size size, originParentSize, parentSize;
};