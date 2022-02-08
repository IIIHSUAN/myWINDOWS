#pragma once

#include "App/App.h"

class Painter;

class PainterWindow : public Window
{
public:
	PainterWindow(std::wstring _name, Pos _pos, Size _size) : Window(_name, _pos, _size) {}
};


class Painter :public App
{
public:
	Painter(Pos pos = { 2,1 }, UINT id = 0) : App(AppCollection::Painter, id) {
		push_window(window, PainterWindow(L"Painter", pos, { 40,MY_WINDOW_HEIGHT - 10 }));

		window->setMouseMoveCallback([this](MouseMoveEvent& e) {
			e.setPos({ e.getMouseX() + e.getOffsetX() - window->getX() , e.getMouseY() + e.getOffsetY() - window->getY() });

			if (e.getIsPrs() &&
				e.getMouseX() > 0 && e.getMouseX() < window->getSize().width - 1 &&
				e.getMouseY() > 0 && e.getMouseY() < window->getSize().height - 1)
				window->getCanvas().line(e.getMouseX() + 2, e.getMouseY() + 2, L"❉", 1);
		});
	}

private:
	std::shared_ptr<Window> window;
};
