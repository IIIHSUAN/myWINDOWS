#pragma once

#include <string>

#include "Struct.h"

class Window;

struct CharImage
{
	const wchar_t* data;
	Pos pos;
	Size size;
	CharImage(const wchar_t* data,Pos pos, Size size) :data(data), pos(pos), size(size) {}
};

class Canvas
{
public:
	Canvas() {}
	Canvas(Pos pos, Size size, bool isFrame = false, wchar_t flushChar = L' ');  // on absolute zero

	void flush(wchar_t newChar = 0);
	
	void animate();

	void line(int X, int Y, const int showLen, const wchar_t * str, const int len);
	void line(int X, int Y, const wchar_t * str, const int len);
	void line(int X, int Y, std::wstring s);
	void lineCenter(int X, int Y, std::wstring& str);
	inline void canvasCenterLine(std::wstring& str) { lineCenter(size.width / 2, size.height / 2, str); }

	void renderWithRel(Canvas& frontl);  // rel pos
	void renderWindow(Window& window);

	void setBackground(CharImage& source);
	inline void setPos(Pos& _pos) { pos = _pos; }
	inline void setSize(Size& _size) { size = _size; }  // resize
	inline void setIsFrame(bool b) { isFrame = b; }

	inline Pos& getPos() { return pos; }
	inline Size& getSize() { return size; }
	inline bool& getIsFrame() { return isFrame; }
	inline std::wstring& getCanvas() { return canvas; }
	inline const wchar_t* getConstCanvas() { return canvas.c_str(); }
private:
	wchar_t flushChar;
	std::wstring canvas;
	bool isFrame;
	Pos pos;
	Size size;
};