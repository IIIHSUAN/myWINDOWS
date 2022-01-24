#pragma once

#include "App/App.h"

class Painter;

class PainterWindow : public Window
{
public:
	PainterWindow(int _id, std::wstring _name, Pos _pos, Size _size) : Window(_id, _name, _pos, _size) {}
};


class Painter :public App
{
public:
	Painter(Pos pos = { 2,1 }) : App(AppCollection::Painter) {
		push_window(window, PainterWindow(0, L"Painter", pos, { 40,MY_WINDOW_HEIGHT - 10 }));

		window->setMouseMoveCallback([this](MouseMoveEvent& e) {
			e.setPos({ e.getMouseX() - window->getX() , e.getMouseY() - window->getY() });

			if (e.getIsPrs() &&
				e.getMouseX() > 0 && e.getMouseX() < window->getSize().width - 1 &&
				e.getMouseY() > 0 && e.getMouseY() < window->getSize().height - 1)
			{
				window->getCanvas().line(e.getMouseX(), e.getMouseY(), L"❉", 1);
				return true;
			}
			return false;
		});
	}

private:
	std::shared_ptr<Window> window;
};
