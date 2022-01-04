#pragma once
#include <windows.h>

#include "Event.h"
#include <stdio.h>
#include <string>

class MouseMoveEvent :public Event
{
public:
	inline MouseMoveEvent(int x, int y, int offsetX, int offsetY, bool isMousePressed) : Event(EventType::mouseMove), isMousePressed(isMousePressed), pos({ x,y }), offset({ offsetX,offsetY }) {}

	inline bool& getIsPressed() { return isMousePressed; }
	inline int& getMouseX() { return pos.x; }
	inline int& getMouseY() { return pos.y; }
	inline int& getOffsetX() { return offset.x; }
	inline int& getOffsetY() { return offset.y; }

	inline void setOffsetX(int i) { offset.x = i; }
	inline void setOffsetY(int i) { offset.y = i; }

	inline void setPos(Pos _pos) { pos = _pos; }
private:
	Pos pos, offset;
	bool isMousePressed;
};


class MousePrsEvent :public Event
{
public:
	inline MousePrsEvent(int x, int y, WORD botton) : Event(EventType::mousePrs), pos({ x,y }), botton(botton) {}
	inline int& getMouseX() { return pos.x; }
	inline int& getMouseY() { return pos.y; }

	inline void setPos(Pos _pos) { pos = _pos; }
private:
	Pos pos;
	WORD botton;
};

class MouseRlsEvent :public Event
{
public:
	inline MouseRlsEvent(int x, int y, WORD botton) : Event(EventType::mouseRls), pos({ x,y }), botton(botton) {}
	inline int& getMouseX() { return pos.x; }
	inline int& getMouseY() { return pos.y; }

	inline void setPos(Pos _pos) { pos = _pos; }
private:
	Pos pos;
	WORD botton;
};

class KeyPrsEvent :public Event
{
public:
	inline KeyPrsEvent(WORD key) : Event(EventType::keyPrs), key(key) {}

	inline WORD& getKey() { return key; }
private:
	WORD key;
};
