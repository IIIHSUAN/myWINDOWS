#include "Desktop.h"

#include <Windows.h>

#include "System/System.h"

void DesktopWindow::b()
{
	static bool isFront = false;
	isFront = !isFront;
	bSettings->animate(Animate({ Left(50,vw),Top(50,vh) }, { Width(50,vw),Height(50,vh) }, 250, Easing::easeInOutCubic), [this]() {
		bSettings->setZindex(isFront ? FRONT(0) : 3);
		bSettings->getAnimate().sleep(500);

		bSettings->animate(Animate({ Right(50,vw),Bottom(50,vh) }, { Width(50,vw),Height(50,vh) }, 250, Easing::easeInOutCubic), [this]() {
			bSettings->setZindex(isFront ? FRONT(0) : 0);
			b();
			bSettings->getAnimate().sleep(250);
			lTime->toggleAnimateStatus();
		});
	});
}

DesktopWindow::DesktopWindow(int _id, std::wstring _name, Pos _pos, Size _size)
	: Window(_id, _name, _pos, _size, L'░')
{
	/* Elements ************************************************************/

	push_elements(iDoge, Image(CharImage({ backgroundData , {Left(15,vw),Bottom(1,px)}, {29,11} }), *this));

	push_elements(lTime, Label(L"Time", { Left(10,vh),Top(8,px) },  *this, false));
	lTime->animate(Animate({ Left(15),Bottom(1) }, 500), [this]() {
		a();
	});
	lTime->toggleAnimateStatus();

	push_elements(bSettings, Button(L"Settings", { Left(1,px,absolute),Bottom(10,vh,absolute) }, *this, true));
	bSettings->onclick([this]() {
		System::get().createApp(AppCollection::Settings);
		return true;
	});
	b();
	bSettings->onhover([this]() {
		if (bSettings->getInfo().mouseHover == ElementsInfo::active)
			bSettings->setString(L" Set Parameters ", L'#'), bSettings->isAnimatePause(true);
		else
			bSettings->setString(L"Settings"), bSettings->isAnimatePause(false);
		return true;
	});

	push_elements(bPainter, Button(L"Painter", { Right(3),Top(25,vh) },  *this, true));
	bPainter->onhover([this]() {
		bPainter->setString(bPainter->getInfo().mouseHover == ElementsInfo::active ? L"Open Painter ?" : L"Painter");
		return true;
	});
	bPainter->onclick([]() {
		System::get().createApp(AppCollection::Painter);
		return true;
	});

	push_elements(bChess, Button(L"Chess", { Right(3),Top(50,vh) },  *this, true));
	bChess->onhover([this]() {
		if (bChess->getInfo().mouseHover == ElementsInfo::active) bChess->setString(L"Play ?", L'|'); else bChess->setString(L"Chess");
		return true;
	});
	bChess->onclick([]() {
		System::get().createApp(AppCollection::Chess);
		return true;
	});

	push_elements(bAnimate, Button(L"  Animate  ", { Left(5,px,relative),Bottom(4,px,relative) },  *this, false, {3,1}));
	bAnimate->onclick([this]() {
		lTime->toggleAnimateStatus();
		bSettings->toggleAnimateStatus();
		return true;
	});

	/* Window pollingCallback ************************************************************/

	setPollingCallback([this]() {
		static int i = 0;
		i = ++i % int(MY_UPDATE_FREQ);

		std::time_t now = std::time(0);
		struct tm t;
		auto lt = localtime_s(&t, &now);

		std::wstring str = std::to_wstring(t.tm_year + 1900) + L' ' + std::to_wstring(t.tm_mon + 1) + L'/' + std::to_wstring(t.tm_mday) + L' ' +
			(t.tm_hour < 10 ? L"0" : L"\0") + std::to_wstring(t.tm_hour) + L':' +
			(t.tm_min < 10 ? L"0" : L"\0") + std::to_wstring(t.tm_min) + L':' +
			(t.tm_sec < 10 ? L"0" : L"\0") + std::to_wstring(t.tm_sec) +
			(int(i*MY_UPDATE_PERIOD) / 10 < 10 ? L":0" : L":") + std::to_wstring(int(i*MY_UPDATE_PERIOD) / 10);

		lTime->setString(str);
	});
}

void DesktopWindow::a()
{
	lTime->animate(Animate({ Left(20,vw,relative),Bottom(5,px,absolute) }, 500), [this]() {
		lTime->setZindex(0);
		lTime->animate(Animate({ Left(10,vw,relative),Bottom(50,vh,relative) }, 500), [this]() {
			lTime->setZindex(FRONT(0));
			a();
		});
	});
}