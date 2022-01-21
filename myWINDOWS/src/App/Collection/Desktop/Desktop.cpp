#include "Desktop.h"

#include <Windows.h>

#include "AppHandler/AppHandler.h"

#define DESKTOP_CALLBACK(eleClass, eleName, code) ELEMENTS_CALLBACK(Desktop, desktop, DesktopWindow, window, eleClass, eleName, code)

DesktopWindow::DesktopWindow(int _id, std::wstring _name, Pos _pos, Size _size)
	: Window(_id, _name, _pos, _size, L'░')
{
	PUSH_ELEMENTS(iBackground, Image(CharImage({ backgroundData , {Left(15,vw,relative),Bottom(1,px,relative)}, {36,11} }), getPos(), getSize()));

	PUSH_ELEMENTS(lTime, Label(L"  Time  ", { Left(10,vh,absolute),Top(10,vw,absolute) }, getPos(), getSize()));
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

	PUSH_ELEMENTS(bSettings, Button(L"  Settings  ", { Right(3,px,relative),Bottom(2,px,relative) }, getPos(), getSize(), true));
	bSettings->onclick([this]() {
		AppHandler::get().createApp(AppCollection::Settings);
		return true;
	});

	PUSH_ELEMENTS(bPainter, Button(L"  Painter   ", { Right(3,px,relative),Bottom(8,px,relative) }, getPos(), getSize(), true));
	bPainter->onhover([this]() {
		if (bPainter->getInfo().mouseHover == InfoType::Active)
			bPainter->setString(L"Open Painter ?", L'|');
		else
			bPainter->setString(L"  Painter   ");
		return false;
	});
	bPainter->onclick([]() {
		AppHandler::get().createApp(AppCollection::Painter);
		return true;
	});

	PUSH_ELEMENTS(bChess, Button(L"   Chess    ", { Right(3,px,relative),Bottom(14,px,relative) }, getPos(), getSize(), true));
	bChess->onhover([this]() {

		if (bChess->getInfo().mouseHover == InfoType::Active)
			bChess->setString(L"Play ?", L'|');
		else
			bChess->setString(L"   Chess    ");
		return false;
	});
	bChess->onclick([]() {
		AppHandler::get().createApp(AppCollection::Chess);
		return true;
	});

	PUSH_ELEMENTS(bAnimate, Button(L"  Animate  ", { Left(5,px,relative),Bottom(4,px,relative) }, getPos(), getSize(), false, {3,1}));
	bAnimate->onclick([this]() {

		lTime->setZindex(FRONT(0));
		lTime->animate(Animate(Pos({ 20,8 }), 0.5f), [this]() {
			lTime->animate(Animate(Pos({ 4,2 }), 0.2f), [this]() {
				lTime->animate(Animate(Pos({ 10,25 }), 0.5f), [this]() {
					lTime->animate(Animate(Pos({ 5,5 }), 1.0f));
				});
			});
		});
		return true;
	});
	
	Window::refresh();
}