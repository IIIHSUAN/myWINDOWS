#include "Desktop.h"

#include <Windows.h>

#include "AppHandler/AppHandler.h"

#define DESKTOP_CALLBACK(eleClass, eleName, code) ELEMENTS_CALLBACK(Desktop, desktop, DesktopWindow, window, eleClass, eleName, code)

DesktopWindow::DesktopWindow(int _id, std::wstring _name, Pos _pos, Size _size)
	: Window(_id, _name, _pos, _size)
{
	Window::getCanvas().setBackground(background);

	PUSH_ELEMENTS(bSettings, Button(L"  Settings  ", { 73, 4 }, true));
	bSettings->onclick([](Button& b) {
		AppHandler::get().createApp(AppCollection::Settings);
		return true;
	});

	PUSH_ELEMENTS(bPainter, Button(L"  Painter   ", { 73, 9 }, true));
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

	PUSH_ELEMENTS(bChess, Button(L"   Chess    ", { 73, 14 }, true));
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
}

void Desktop::run()
{
	// time thread
	while (!windowVec.empty())
	{
		std::time_t now = std::time(0);
		struct tm t;
		auto lt = localtime_s(&t, &now);

		std::wstring str = L"  " + std::to_wstring(t.tm_year + 1900) + L' ' + std::to_wstring(t.tm_mon + 1) + L'/' + std::to_wstring(t.tm_mday) + L' ' +
			(t.tm_hour < 10 ? L"0" : L"\0") + std::to_wstring(t.tm_hour) + L':' +
			(t.tm_min < 10 ? L"0" : L"\0") + std::to_wstring(t.tm_min) + L':' +
			(t.tm_sec < 10 ? L"0" : L"\0") + std::to_wstring(t.tm_sec) + L"  ";

		windowVec[0]->getCanvas().line(40, 8, str.c_str(), str.length()), App::setPollingUpdate(true);

		Sleep(1000);
	}
}