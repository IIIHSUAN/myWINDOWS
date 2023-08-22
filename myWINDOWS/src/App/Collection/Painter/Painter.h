#pragma once

#include "App/App.h"

class PainterWindow : public Window
{
public:
	PainterWindow(std::wstring _name, Pos _pos, Size _size) : Window(_name, _pos, _size) {
		push_elements(div, Div({ Left(0),Top(0) }, { _size.width,_size.height }, *this));
	}

	std::shared_ptr<Div> div;
};


class Painter :public App
{
public:
	Painter(Pos pos = { 2,1 }, UINT id = 0) : App(AppCollection::Painter, id) {
		push_window(painterWindow, PainterWindow(L"Painter", pos, { 40,MY_WINDOW_HEIGHT - 10 }));

		painterWindow->setMouseMoveCallback([this](MouseMoveEvent& e) {
			if (!e.getIsPrs())
				return;

			painterWindow->setTitle(L"Painter (" + std::to_wstring(e.getMouseX()) + L", " + std::to_wstring(e.getMouseY()) + L")");
			((PainterWindow&)(*painterWindow)).div->getCanvas().line(e.getMouseX(), e.getMouseY(), L"❉", 1);
		});

		painterWindow->setResizeCallback([this](WindowResizeEvent& e) {
			((PainterWindow&)(*painterWindow)).div->getCanvas().setSize(e.getSize());
		});
	}

private:
	std::shared_ptr<Window> painterWindow;
};
