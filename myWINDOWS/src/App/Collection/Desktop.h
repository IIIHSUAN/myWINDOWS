#pragma once

#include "../App.h"
#include "Painter.h"

#include <ctime>

class Desktop;

class DesktopWindow :public Window
{
public:
	DesktopWindow(Desktop& app, int _id, std::wstring _name, Pos _pos, Size _size);
private:
	Desktop& app;

	std::shared_ptr<Button>bPainter, bChess;

	const wchar_t* backgroundData = L"\
░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░\
░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░\
░░░░░░░░▌▒█░░░░░░░░░░░▄▀▒▌░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░\
░░░░░░░░▌▒▒█░░░░░░░░▄▀▒▒▒▐░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░\
░░░░░░░▐▄▀▒▒▀▀▀▀▄▄▄▀▒▒▒▒▒▐░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░\
░░░░░▄▄▀▒░▒▒▒▒▒▒▒▒▒█▒▒▄█▒▐░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░\
░░░▄▀▒▒▒░░░▒▒▒░░░▒▒▒▀██▀▒▌░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░\
░░▐▒▒▒▄▄▒▒▒▒░░░▒▒▒▒▒▒▒▀▄▒▒▌░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░\
░░▌░░▌█▀▒▒▒▒▒▄▀█▄▒▒▒▒▒▒▒█▒▐░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░\
░▐░░░▒▒▒▒▒▒▒▒▌██▀▒▒░░░▒▒▒▀▄▌░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░\
░▌░▒▄██▄▒▒▒▒▒▒▒▒▒░░░░░░▒▒▒▒▌░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░\
▀▒▀▐▄█▄█▌▄░▀▒▒░░░░░░░░░░▒▒▒▌░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░";
	CharImage background = { backgroundData ,{6,3}, {66,12} };
};

class Desktop :public App
{
public:
	Desktop() : App(AppCollection::Desktop, windowVec) {
		CREATE_WINDOW(windowVec, DesktopWindow(*this, 0, L"Desktop", { 2,1 }, { MY_WINDOW_WIDTH - 5, MY_WINDOW_HEIGHT - 10 }));
	}
	virtual void run() override;
private:
	std::vector<std::shared_ptr<Window>> windowVec;
};