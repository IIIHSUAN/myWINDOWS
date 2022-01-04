#pragma once

#include "../App.h"

class Painter;

class PainterWindow : public Window
{
public:
	PainterWindow(int _id, std::wstring _name, Pos _pos, Size _size) : Window(_id, _name, _pos, _size) {}
};


class Painter :public App
{
public:
	Painter() : App(AppCollection::Painter, windowVec) {
		CREATE_WINDOW(windowVec, PainterWindow(0, L"Painter", { 2,1 }, { MY_WINDOW_WIDTH - 15,MY_WINDOW_HEIGHT - 10 }));

		windowVec[0]->setMouseMoveCallback([this](MouseMoveEvent e) {
			auto& window = windowVec[0];
			e.setPos({ e.getMouseX() - window->getX() , e.getMouseY() - window->getY() });

			if (e.getIsPrs() &&
				e.getMouseX() > 0 && e.getMouseX() < window->getWidth() - 1 &&
				e.getMouseY() > 0 && e.getMouseY() < window->getHeight() - 1)
			{
				getWindowVec()[0]->getCanvas().line(e.getMouseX(), e.getMouseY(), L"❉", 1);
				return true;
			}
			return false;
		});
	}

private:
	std::vector<std::shared_ptr<Window>> windowVec;
};
