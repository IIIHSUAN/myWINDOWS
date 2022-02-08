#pragma once

#include <windows.h>
#include <string>

#define POLLING_MAP(x) Input::get().getPollingMap()[UINT(PollingInput::x)]

class MyMouse {
public:
	int X = 0, Y = 5, offsetX = 0, offsetY = 0;
	bool isPrs = false;

	enum Speed { normal = 1, fast = 2, vfast = 4 };
	Speed speed = normal;
	void changeSpeed() { speed = speed == normal ? fast : (speed == fast ? vfast : normal); }

	static inline MyMouse& get() { return *mouse; }
private:
	static MyMouse* mouse;
};

enum class PollingInput :unsigned int {
	unknown = 0, error,
	/**********************************************/

	// keyboard
	shift, ctrl, spacebar, enter,
	up, down, left, right,
	A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
	num0, num1, num2, num3, num4, num5, num6, num7, num8, num9,
	pad0, pad1, pad2, pad3, pad4, pad5, pad6, pad7, pad8, pad9,

	// mouse
	mouse_l1, mouse_l2, mouse_l3, mouse_l4, mouse_r1,

	/**********************************************/
	__SIZE__
};


class Input
{
public:
	Input();
	void run();

	static inline Input& get() { return *input; }

	void keyMapping(WORD& key, bool isPrs);
	PollingInput mouseMapping(DWORD& state);

	void resetInputSettings();

	inline bool* getPollingMap() { return pollingMap; }
private:
	static Input* input;

	DWORD events, num;
	INPUT_RECORD buffer;
	HANDLE inHandle = GetStdHandle(STD_INPUT_HANDLE);

	bool pollingMap[unsigned int(PollingInput::__SIZE__)];

	bool isResizeThread = false;
	unsigned int resizeCount = 0;
	COORD resize;
};

/************************************************************************/

class ResizeEvent;

class Output
{
public:
	Output();
	void display(const std::wstring& pixels);
	void setFontSize(int _fontsizeY);
	inline int getFontSize() { return fontsizeY; }

	void onResizeEvent(ResizeEvent& e);

	static short width, height;

	static inline Output& get() { return *output; }
private:
	static Output* output;
	int fontsizeX = 0, fontsizeY = 14;

	DWORD num;
	CONSOLE_FONT_INFOEX cfi;
	HANDLE outHandle = GetStdHandle(STD_OUTPUT_HANDLE);
};