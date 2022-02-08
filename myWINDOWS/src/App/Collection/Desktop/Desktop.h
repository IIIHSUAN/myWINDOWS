#pragma once

#include "App/App.h"

#include <ctime>

class Desktop;

class DesktopWindow :public Window
{
public:
	friend Desktop;
	DesktopWindow(std::wstring _name, Pos _pos, Size _size);

	void a();
	void b();
private:
	std::shared_ptr<Button>bDesktop, bSettings, bPainter, bChess, bAnimate, b0, b1, b2, b3, b4;
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
	Desktop(Pos pos = { 3,1 }, UINT id = 0) : App(AppCollection::Desktop, id) {
		push_window(desktopWindow, DesktopWindow(L"Desktop id: "+ std::to_wstring(getId()), pos, { MY_WINDOW_WIDTH - 50, MY_WINDOW_HEIGHT - 5 }));

		push_window(devToolWindow, DevToolWindow({ MY_WINDOW_WIDTH - 80 + pos.x, 2 + pos.y }, { 40, MY_WINDOW_HEIGHT - 10 }, false));
		auto& desktop = (DesktopWindow &)*desktopWindow;
		auto& s = desktop.bSettings;
		auto& t = desktop.lTime;
		
		devTool_push(devToolWindow, System::get().getFps());

		devTool_push(devToolWindow, POLLING_MAP(A));
		devTool_push(devToolWindow, POLLING_MAP(S));
		devTool_push(devToolWindow, POLLING_MAP(D));
		devTool_push(devToolWindow, POLLING_MAP(W));
		devTool_push(devToolWindow, POLLING_MAP(spacebar));
		devTool_push(devToolWindow, POLLING_MAP(shift));
		devTool_push(devToolWindow, POLLING_MAP(ctrl));
		devTool_push(devToolWindow, POLLING_MAP(mouse_l1));
		devTool_push(devToolWindow, POLLING_MAP(mouse_l2));
		devTool_push(devToolWindow, POLLING_MAP(mouse_l3));
		devTool_push(devToolWindow, POLLING_MAP(mouse_l4));
		devTool_push(devToolWindow, POLLING_MAP(mouse_r1));

		//devTool_push(devToolWindow, s->getZindex());
		//devTool_push(devToolWindow, t->getZindex());
	}
private:
	std::shared_ptr<Window> desktopWindow, devToolWindow;
};