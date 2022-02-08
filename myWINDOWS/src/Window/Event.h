#pragma once

#include "Graphics/Struct.h"

enum class Status { none, handled, needUpdate, refresh };

class Event
{
public:
	enum Type { unknown, mouseMove, mousePrs, mouseRls, keyPrs, recv, resize, windowResize, shutdown };

	Event(Type type) :type(type) {}
	
	inline Type& getType() { return type; }
protected:
	Type type = unknown;
};

/* MyMouse Move Event ****************************************************/

class MouseMoveEvent :public Event
{
public:
	MouseMoveEvent(int x, int y, int offsetX, int offsetY, bool isMousePrs) 
		: Event(Event::mouseMove), isMousePrs(isMousePrs), pos({ x,y }), offset({ offsetX,offsetY }) {}

	inline bool& getIsPrs() { return isMousePrs; }
	inline int& getMouseX() { return pos.x; }
	inline int& getMouseY() { return pos.y; }
	inline int& getOffsetX() { return offset.x; }
	inline int& getOffsetY() { return offset.y; }

	inline void setOffsetX(int i) { offset.x = i; }
	inline void setOffsetY(int i) { offset.y = i; }

	inline void setPos(Pos _pos) { pos = _pos; }
private:
	Pos pos, offset;
	bool isMousePrs;
};

/* MyMouse Press Event ****************************************************/

class MousePrsEvent :public Event
{
public:
	MousePrsEvent(int x, int y, PollingInput botton) : Event(Event::mousePrs), pos({ x,y }), botton(botton) {}
	inline int& getMouseX() { return pos.x; }
	inline int& getMouseY() { return pos.y; }
	inline PollingInput& getBotton() { return botton; }

	inline void setPos(Pos _pos) { pos = _pos; }
private:
	Pos pos;
	PollingInput botton;
};

/* MyMouse Release Event ****************************************************/

class MouseRlsEvent :public Event
{
public:
	MouseRlsEvent(int x, int y) : Event(Event::mouseRls), pos({ x,y }) {}
	inline int& getMouseX() { return pos.x; }
	inline int& getMouseY() { return pos.y; }

	inline void setPos(Pos _pos) { pos = _pos; }
private:
	Pos pos;
};

/* Key Press Event ****************************************************/

enum class KeySet {
	unknown,
	backspace, tab, enter, shift, ctrl, alt, pause, capslock, numlock, scrolllock, esc, spacebar,
	l_shift, r_shift, l_ctrl, r_ctrl, l_menu, r_menu, l_window, r_window,
	page_up, page_down, home, end, left, up, right, down,
	print_screen, insert, del,
	num0, num1, num2, num3, num4, num5, num6, num7, num8, num9,
	pad0, pad1, pad2, pad3, pad4, pad5, pad6, pad7, pad8, pad9, plus, minus, multiple, divide, dot,
	A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
	F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,

	browse_back, browse_forward, browse_refresh, browse_stop, browse_search, browse_home,
	vol_mute, vol_down, vol_up, media_next, media_prev, media_stop, media_play_pause
};
class KeyPrsEvent :public Event
{
public:
	inline KeyPrsEvent(unsigned short key, WCHAR unicodeChar) : Event(Event::keyPrs), key(key), unicodeChar(unicodeChar) {}

	inline WCHAR getChar() { return unicodeChar; }
	inline KeySet getKey() { return toKeySet(key); }

	static inline char toChar(unsigned short& key) {  // deprecated
		switch (key)
		{
		default: return char(key);
		case 0x60: return '0';
		case 0x61: return '1';
		case 0x62: return '2';
		case 0x63: return '3';
		case 0x64: return '4';
		case 0x65: return '5';
		case 0x66: return '6';
		case 0x67: return '7';
		case 0x68: return '8';
		case 0x69: return '9';
		case 0x6A: return '*';
		case 0x6B: return '+';
		case 0x6D: return '-';
		case 0x6E: return '.';
		case 0x6F: return '/';
		}
	}
	static inline KeySet toKeySet(unsigned short& key) {
		switch (key)
		{
		default: return KeySet::unknown;
		case 0x08: return KeySet::backspace;
		case 0x09: return KeySet::tab;
		case 0x0D: return KeySet::enter;
		case 0x10: return KeySet::shift;
		case 0x11: return KeySet::ctrl;
		case 0x12: return KeySet::alt;
		case 0x13: return KeySet::pause;
		case 0x14: return KeySet::capslock;
		case 0x1B: return KeySet::esc;
		case 0x20: return KeySet::spacebar;
		case 0x21: return KeySet::page_up;
		case 0x22: return KeySet::page_down;
		case 0x23: return KeySet::end;
		case 0x24: return KeySet::home;
		case 0x25: return KeySet::left;
		case 0x26: return KeySet::up;
		case 0x27: return KeySet::right;
		case 0x28: return KeySet::down;
		case 0x2C: return KeySet::print_screen;
		case 0x2D: return KeySet::insert;
		case 0x2E: return KeySet::del;
		case 0x30: return KeySet::num0;
		case 0x31: return KeySet::num1;
		case 0x32: return KeySet::num2;
		case 0x33: return KeySet::num3;
		case 0x34: return KeySet::num4;
		case 0x35: return KeySet::num5;
		case 0x36: return KeySet::num6;
		case 0x37: return KeySet::num7;
		case 0x38: return KeySet::num8;
		case 0x39: return KeySet::num9;
		case 0x41: return KeySet::A;
		case 0x42: return KeySet::B;
		case 0x43: return KeySet::C;
		case 0x44: return KeySet::D;
		case 0x45: return KeySet::E;
		case 0x46: return KeySet::F;
		case 0x47: return KeySet::G;
		case 0x48: return KeySet::H;
		case 0x49: return KeySet::I;
		case 0x4A: return KeySet::J;
		case 0x4B: return KeySet::K;
		case 0x4C: return KeySet::L;
		case 0x4D: return KeySet::M;
		case 0x4E: return KeySet::N;
		case 0x4F: return KeySet::O;
		case 0x50: return KeySet::P;
		case 0x51: return KeySet::Q;
		case 0x52: return KeySet::R;
		case 0x53: return KeySet::S;
		case 0x54: return KeySet::T;
		case 0x55: return KeySet::U;
		case 0x56: return KeySet::V;
		case 0x57: return KeySet::W;
		case 0x58: return KeySet::X;
		case 0x59: return KeySet::Y;
		case 0x5A: return KeySet::Z;
		case 0x60: return KeySet::pad0;
		case 0x61: return KeySet::pad1;
		case 0x62: return KeySet::pad2;
		case 0x63: return KeySet::pad3;
		case 0x64: return KeySet::pad4;
		case 0x65: return KeySet::pad5;
		case 0x66: return KeySet::pad6;
		case 0x67: return KeySet::pad7;
		case 0x68: return KeySet::pad8;
		case 0x69: return KeySet::pad9;
		case 0x6A: return KeySet::multiple;
		case 0x6B: return KeySet::plus;
		case 0x6D: return KeySet::minus;
		case 0x6E: return KeySet::dot;
		case 0x6F: return KeySet::divide;
		case 0x90: return KeySet::numlock;
		case 0x91: return KeySet::scrolllock;
		case 0xA0: return KeySet::l_shift;
		case 0xA1: return KeySet::r_shift;
		case 0xA2: return KeySet::l_ctrl;
		case 0xA3: return KeySet::r_ctrl;
		case 0xA4: return KeySet::l_menu;
		case 0xA5: return KeySet::r_menu;

		case 0xA6: return KeySet::browse_back;
		case 0xA7: return KeySet::browse_forward;
		case 0xA8: return KeySet::browse_refresh;
		case 0xA9: return KeySet::browse_stop;
		case 0xAA: return KeySet::browse_search;
		case 0xAC: return KeySet::browse_home;
		case 0xAD: return KeySet::vol_mute;
		case 0xAE: return KeySet::vol_down;
		case 0xAF: return KeySet::vol_up;
		case 0xB0: return KeySet::media_next;
		case 0xB1: return KeySet::media_prev;
		case 0xB2: return KeySet::media_stop;
		case 0xB3: return KeySet::media_play_pause;
		}
	}
private:
	unsigned short key;
	WCHAR unicodeChar;
};

/* Resize Event ****************************************************/

class ResizeEvent :public Event
{
public:
	ResizeEvent(short width, short height, float rWidth, float rHeight) : Event(Event::resize), width(width), height(height), rWidth(rWidth), rHeight(rHeight) {}
	
	inline short getWidth() { return width; }
	inline short getHeight() { return height; }
	inline float getWidthRatio() { return rWidth; }
	inline float getHeightRatio() { return rHeight; }
private:
	short width, height;
	float rWidth, rHeight;
};

/* Window Resize Event (only for Window dispatch) ****************************************************/

class WindowResizeEvent :public Event
{
public:
	WindowResizeEvent(Pos originPos, Size originSize, Pos pos, Size size) 
		: Event(Event::windowResize), originPos(originPos), originSize(originSize), pos(pos), size(size) {}

	inline const Pos& getPos() { return pos; }
	inline const Size& getSize() { return size; }
	inline const Pos& getOriginPos() { return originPos; }
	inline const Size& getOriginSize() { return originSize; }
private:
	Pos originPos, pos;
	Size originSize, size;
};

/* Network Recieve Event ****************************************************/

class RecvEvent :public Event
{
};

/* Shut Down Event ****************************************************/

class ShutdownEvent :public Event
{
public:
	ShutdownEvent() : Event(Event::shutdown) {}
};