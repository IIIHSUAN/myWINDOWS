#pragma once

#include "App/App.h"
#include "../Painter/Painter.h"

#include <ctime>

class Desktop;

class DesktopWindow :public Window
{
public:
	friend Desktop;
	DesktopWindow(int _id, std::wstring _name, Pos _pos, Size _size);

	void a();
	void b();
private:
	std::shared_ptr<Button>bSettings, bPainter, bChess, bAnimate, b0, b1, b2, b3, b4;
	std::shared_ptr<Image> iDoge;
	std::shared_ptr<Label> lTime;

	const wchar_t* backgroundData = L"\
        ▄▀▄             ▄   \
        ▌▒█           ▄▀▒▌  \
        ▌▒▒█        ▄▀▒▒▒▐  \
       ▐▄▀▒▒▀▀▀▀▄▄▄▀▒▒▒▒▒▐  \
     ▄▄▀▒░▒▒▒▒▒▒▒▒▒█▒▒▄█▒▐  \
   ▄▀▒▒▒░░░▒▒▒░░░▒▒▒▀██▀▒▒▌ \
  ▐▒▒▒▄▄▒▒▒▒░░░▒▒▒▒▒▒▒▀▄▒▒▌ \
  ▌░░▌█▀▒▒▒▒▒▄▀█▄▒▒▒▒▒▒▒█▒▐ \
 ▐░░░▒▒▒▒▒▒▒▒▌██▀▒▒░░░▒▒▒▀▄▌\
 ▌░▒▄██▄▒▒▒▒▒▒▒▒▒░░░░░░▒▒▒▒▌\
▀▒▀▐▄█▄█▌▄░▀▒▒░░░░░░░░░░▒▒▒▌";

};

class Desktop :public App
{
public:
	Desktop(Pos pos = { 3,1 }) : App(AppCollection::Desktop) {
		push_window(desktopWindow, DesktopWindow(0, L"Desktop", pos, { MY_WINDOW_WIDTH - 50, MY_WINDOW_HEIGHT - 5 }));

		push_window(devToolWindow, DevToolWindow(0, { MY_WINDOW_WIDTH - 40, 2 }, { 40,20 }, false));
		auto& desktop = (DesktopWindow &)*desktopWindow;
		auto& s = desktop.bSettings;
		auto& t = desktop.lTime;

		devTool_push(devToolWindow, System::get().getFps());

		//devTool_push(devToolWindow, s->getAnimate().sleepTime);
		//devTool_push(devToolWindow, s->getZindex());
		//devTool_push(devToolWindow, t->getZindex());
	}
private:
	std::shared_ptr<Window> desktopWindow, devToolWindow;
};