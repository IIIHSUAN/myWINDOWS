#include "Desktop.h"

#include <Windows.h>

#include "AppHandler/AppHandler.h"

#define DESKTOP_CALLBACK(eleClass, eleName, code) ELEMENTS_CALLBACK(Desktop, desktop, DesktopWindow, window, eleClass, eleName, code)

DesktopWindow::DesktopWindow(int _id, std::wstring _name, Pos _pos, Size _size)
	: Window(_id, _name, _pos, _size)
{
	PUSH_ELEMENTS(lTime, Label(L"  Time  ", { 0,0,40,11,Position::relative }, getSize()));
	setPollingCallback([this]() {
		static int i = 0;
		i = ++i % int(MY_UPDATE_FREQ);

		std::time_t now = std::time(0);
		struct tm t;
		auto lt = localtime_s(&t, &now);

		std::wstring str = L"  " + std::to_wstring(t.tm_year + 1900) + L' ' + std::to_wstring(t.tm_mon + 1) + L'/' + std::to_wstring(t.tm_mday) + L' ' +
			(t.tm_hour < 10 ? L"0" : L"\0") + std::to_wstring(t.tm_hour) + L':' +
			(t.tm_min < 10 ? L"0" : L"\0") + std::to_wstring(t.tm_min) + L':' +
			(t.tm_sec < 10 ? L"0" : L"\0") + std::to_wstring(t.tm_sec) + 
			(int(i*MY_UPDATE_PERIOD) / 10 < 10 ? L":0" : L":") + std::to_wstring(int(i*MY_UPDATE_PERIOD) / 10) + L"  ";

		lTime->setString(str);
	});

	PUSH_ELEMENTS(iBackground, Image(CharImage({ backgroundData , { 1,0,0,1,Position::relative }, {99,15} }), getSize()));

	PUSH_ELEMENTS(bSettings, Button(L"  Settings  ", { 0,0,3,2,Position::relative }, getSize(), true));
	bSettings->onclick([](Button& b) {
		AppHandler::get().createApp(AppCollection::Settings);
		return true;
	});

	PUSH_ELEMENTS(bPainter, Button(L"  Painter   ", { 0,0,3,8,Position::relative }, getSize(), true));
	bPainter->onhover([](Button& b) {
		if (b.getInfo().mouseHover == InfoType::Active)
			b.setString(L"Open Painter ?", L'|');
		else
			b.setString(L"  Painter   ");
		return false;
	});
	bPainter->onclick([](Button& b) {
		AppHandler::get().createApp(AppCollection::Painter);
		return true;
	});

	PUSH_ELEMENTS(bChess, Button(L"   Chess    ", { 0,0,3,14,Position::relative }, getSize(), true));
	bChess->onhover([](Button& b) {
		if (b.getInfo().mouseHover == InfoType::Active)
			b.setString(L"Play ?", L'|');
		else
			b.setString(L"   Chess    ");
		return false;
	});
	bChess->onclick([](Button& b) {
		AppHandler::get().createApp(AppCollection::Chess);
		return true;
	});

	Window::refresh();
}