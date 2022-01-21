﻿#pragma once

#include "App/App.h"
#include "../Painter/Painter.h"

#include <ctime>

class Desktop;

class DesktopWindow :public Window
{
public:
	DesktopWindow(int _id, std::wstring _name, Pos _pos, Size _size);

private:
	std::shared_ptr<Button>bSettings, bPainter, bChess, bAnimate, b0, b1, b2, b3, b4;
	std::shared_ptr<Image> iBackground;
	std::shared_ptr<Label> lTime;

	const wchar_t* backgroundData = L"\
░░░░░░░░░░░▄▀▄░░░░░░░░░░░░▄▄░░░░░░░\
░░░░░░░░░░░▌▒█░░░░░░░░░░░▄▀▒▌░░░░░░\
░░░░░░░░░░░▌▒▒█░░░░░░░░▄▀▒▒▒▐░░░░░░\
░░░░░░░░░░▐▄▀▒▒▀▀▀▀▄▄▄▀▒▒▒▒▒▐░░░░░░\
░░░░░░░░▄▄▀▒░▒▒▒▒▒▒▒▒▒█▒▒▄█▒▐░░░░░░\
░░░░░░▄▀▒▒▒░░░▒▒▒░░░▒▒▒▀██▀▒▌░░░░░░\
░░░░░▐▒▒▒▄▄▒▒▒▒░░░▒▒▒▒▒▒▒▀▄▒▒▌░░░░░\
░░░░░▌░░▌█▀▒▒▒▒▒▄▀█▄▒▒▒▒▒▒▒█▒▐░░░░░\
░░░░▐░░░▒▒▒▒▒▒▒▒▌██▀▒▒░░░▒▒▒▀▄▌░░░░\
░░░░▌░▒▄██▄▒▒▒▒▒▒▒▒▒░░░░░░▒▒▒▒▌░░░░\
░░░▀▒▀▐▄█▄█▌▄░▀▒▒░░░░░░░░░░▒▒▒▌░░░░";

};

class Desktop :public App
{
public:
	Desktop(Pos pos = { 3,1 }) : App(AppCollection::Desktop) {
		PUSH_WINDOW(window, DesktopWindow(0, L"Desktop", pos, { MY_WINDOW_WIDTH - 50, MY_WINDOW_HEIGHT - 5 }));
	}
private:
	std::shared_ptr<Window> window;
};