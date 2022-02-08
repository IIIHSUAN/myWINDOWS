#include "Desktop.h"

#include <Windows.h>

#include "System/System.h"

void DesktopWindow::b()
{
	static bool isFront = false;
	isFront = !isFront;
	bSettings->animate(Animate({ Left(50,vw),Top(50,vh) }, { Width(50,vw),Height(50,vh) }, 1000, Easing::easeInQuint), [this]() {
		bSettings->setZindex(isFront ? FRONT(0) : 3);
		bSettings->getAnimate().sleep(500);

		bSettings->animate(Animate({ Left(0),Bottom(50,vh) }, { Width(30,px),Height(30,vh) }, 1000, Easing::easeOutQuint), [this]() {
			bSettings->setZindex(0);
			b();
			bSettings->getAnimate().sleep(250);
			lTime->toggleAnimateStatus();
		});
	});
}

DesktopWindow::DesktopWindow(std::wstring _name, Pos _pos, Size _size)
	: Window(_name, _pos, _size, L'░')
{
	/* Elements ************************************************************/

	push_elements(iDoge, Image(CharImage({ backgroundData , {Left(15,vw),Top(1,px)}, {29,11} }), *this));

	push_elements(lTime, Label(L"Time", { Right(10,vw),Top(2,px) }, *this, false));
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
		bSettings->isAnimatePause(bSettings->getInfo().mouseHover == ElementsInfo::active);
		return true;
	});

	push_elements(bDesktop, Button(L"Desktop", { Right(3),Top(20,vh) }, *this, true));
	bDesktop->onclick([]() {
		System::get().createApp(AppCollection::Desktop);
		return true;
	});

	push_elements(bPainter, Button(L"Painter", { Right(3),Top(40,vh) },  *this, true));
	bPainter->onhover([this]() {
		bPainter->setString(bPainter->getInfo().mouseHover == ElementsInfo::active ? L"Open Painter ?" : L"Painter");
		return true;
	});
	bPainter->onclick([]() {
		System::get().createApp(AppCollection::Painter);
		return true;
	});

	push_elements(bChess, Button(L"Chess", { Right(3),Top(60,vh) },  *this, true));
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

		for (auto& App : System::get().getAppList())
			for (auto& window : App->getWindowList())
				if (window->getType() == WindowCollection::devTool)
					((DevToolWindow&)*window).toggleTransparent();
		return true;
	});

	/* Window pollingCallback ************************************************************/

	setPollingCallback([this]() {
		static int i = 0;
		i = ++i % max(int(MY_UPDATE_FREQ), 1);

		std::time_t now = std::time(0);
		struct tm t;
		auto lt = localtime_s(&t, &now);

		std::wstring str = std::to_wstring(t.tm_year + 1900) + L' ' + std::to_wstring(t.tm_mon + 1) + L'/' + std::to_wstring(t.tm_mday) + L' ' +
			(t.tm_hour < 10 ? L"0" : L"\0") + std::to_wstring(t.tm_hour) + L':' +
			(t.tm_min < 10 ? L"0" : L"\0") + std::to_wstring(t.tm_min) + L':' +
			(t.tm_sec < 10 ? L"0" : L"\0") + std::to_wstring(t.tm_sec) +
			(int(i*MY_UPDATE_PERIOD) / 10 < 10 ? L":0" : L":") + std::to_wstring(int(i*MY_UPDATE_PERIOD) / 10);

		lTime->setString(str);

		//
		Pos4 iDogePos4 = iDoge->getCanvas().getPos4();
		bool opcode = false;
		int speed = POLLING_MAP(shift) ? 3 : POLLING_MAP(ctrl) ? 1 : 2;

		if (POLLING_MAP(W))
			iDogePos4.vAttr.value -= speed, opcode = true;
		if (POLLING_MAP(A))
			iDogePos4.hAttr.value -= speed, opcode = true;
		if (POLLING_MAP(S))			 
			iDogePos4.vAttr.value += speed, opcode = true;
		if (POLLING_MAP(D))			 
			iDogePos4.hAttr.value += speed, opcode = true;
		if (POLLING_MAP(spacebar))
			iDogePos4.vAttr.value -= 10, iDoge->animate(Animate(iDogePos4, 250), [this]() { 
				Pos4 iDogePos4 = iDoge->getCanvas().getPos4();
				iDogePos4.vAttr.value += 10;
				iDoge->animate(Animate(iDogePos4, 250));
			});

		if (opcode)
		{
			iDoge->setPos4(iDogePos4);
			return Status::refresh;
		}
		return Status::none;
	});
}

void DesktopWindow::a()
{
	lTime->animate(Animate({ Left(20,vw,relative),Bottom(5,px,absolute) }, 500), [this]() {
		lTime->setZindex(0);
		lTime->animate(Animate({ Right(20,vw,relative),Bottom(50,vh,relative) }, 500), [this]() {
			lTime->setZindex(FRONT(0));
			a();
		});
	});
}