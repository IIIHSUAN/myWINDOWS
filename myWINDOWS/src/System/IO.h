#pragma once

#include <windows.h>

class Mouse {
public:
	int X = 0, Y = 5, offsetX = 0, offsetY = 0;
	bool isPrs = false;

	enum Speed { normal = 1, fast = 2, vfast = 4 };
	Speed speed = normal;
	void changeSpeed() { speed = speed == normal ? fast : (speed == fast ? vfast : normal); }

	static inline Mouse& get() { return *mouse; }
private:
	static Mouse* mouse;
};

class Input
{
public:
	Input();
	void run();

	inline void setIsEventUpdate(bool b) { isEventUpdate = b; }
	inline const bool& getIsEventUpdate() { return isEventUpdate; }

	static inline Input& get() { return *input; }
private:
	static Input* input;

	DWORD events, num;
	INPUT_RECORD buffer;
	HANDLE inHandle = GetStdHandle(STD_INPUT_HANDLE);

	bool isEventUpdate = false;
};

/************************************************************************/

class Output
{
public:
	Output();
	void display(const wchar_t* pixels);
	void setFontSize(int _size);
	inline int getFontSize() { return size; }

	static inline Output& get() { return *output; }
private:
	static Output* output;
	int size = 28;

	DWORD num;
	CONSOLE_FONT_INFOEX cfi;
	HANDLE outHandle = GetStdHandle(STD_OUTPUT_HANDLE);
};