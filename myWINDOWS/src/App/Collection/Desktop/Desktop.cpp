#include "Desktop.h"

#include <Windows.h>

#include "System/System.h"

void DesktopWindow::animTime()
{
	lTime->animate(Animate({ Left(20,vw,relative),Bottom(5,px,absolute) }, 500), [this]() {
		lTime->setZindex(0);
		lTime->animate(Animate({ Right(20,vw,relative),Bottom(50,vh,relative) }, 500), [this]() {
			lTime->setZindex(FRONT(0));
			animTime();
			});
		});
}

void DesktopWindow::animSettings()
{
	static bool isFront = false;
	isFront = !isFront;
	bSettings->animate(Animate({ Left(50,vw),Top(50,vh) }, { Width(50,vw),Height(50,vh) }, 1000, Easing::easeInQuint), [this]() {
		bSettings->setZindex(isFront ? FRONT(0) : 3);
		bSettings->getAnimate().sleep(500);

		bSettings->animate(Animate({ Left(0),Bottom(50,vh) }, { Width(30,px),Height(30,vh) }, 1000, Easing::easeOutQuint), [this]() {
			bSettings->setZindex(0);
			animSettings();
			bSettings->getAnimate().sleep(250);
			lTime->toggleAnimateStatus();
		});
	});
}

void DesktopWindow::animDoge()
{
	// left hide
	iDoge->animate(Animate({ Left(5,vw),Bottom(-5,px) }, 1000,Easing::easeInOutCubic), [this]() {
		iDoge->getAnimate().sleep(500);
		iDoge->setZindex(100);
		// middle show
		iDoge->animate(Animate({ Left(18,vw),Bottom(0,px) }, 2000), [this]() {
			// simulate sleep
			iDoge->animate(Animate({ Left(18,vw),Bottom(0,px) }, 1500, Easing::easeInCubic), [this]() {
				// right hide
				iDoge->animate(Animate({ Left(25,vw),Bottom(-5,px) }, 2000, Easing::easeInCubic), [this]() {
					// right to the back
					iDoge->animate(Animate({ Left(25,vw),Bottom(-10,px) }, 200), [this]() {
						iDoge->setZindex(0);
						animDoge();
					});
				});
			});
		});
	});
}

void DesktopWindow::animJenny()
{
	// jump
	iJenny->animate(Animate({ Left(15, vw), Bottom(12, px) }, 200), [this]() {
		iJenny->animate(Animate({ Left(15, vw), Bottom(16, px) }, 200, Easing::easeInElastic), [this]() {
			// land
			iJenny->animate(Animate({ Left(15, vw), Bottom(13, px) }, 200, Easing::easeOutCubic), [this]() {
				// jump
				iJenny->animate(Animate({ Left(15, vw), Bottom(20, px) }, 300, Easing::easeInCubic), [this]() {
					// land
					iJenny->animate(Animate({ Left(15, vw), Bottom(14, px) }, 300, Easing::easeOutCubic), [this]() {
						// sleep 3000 (anim same place so resize will adjust)
						iJenny->animate(Animate({ Left(15, vw), Bottom(14, px) }, 3000), [this]() {
							// bye run
							iJenny->animate(Animate({ Left(40, vw), Bottom(20, px) }, 2000, Easing::easeInElastic), [this]() {
								// run land
								iJenny->animate(Animate({ Left(80, vw), Bottom(15, px) }, 700), [this]() {
									iJenny->animate(Animate({ Right(-50, px), Bottom(1, px) }, 500), [this]() {
										// to the left
										iJenny->getAnimate().sleep(2000);
										iJenny->setPos4({ Left(-50, px), Bottom(-5, px) });
										iJenny->animate(Animate({ Left(5, vw), Bottom(20, px) }, 500), [this]() {
											animJenny();
										});
									});
								});
							});
						});
					});
				});
			});
		});
	});
}

void DesktopWindow::animHan()
{
	iHan->animate(Animate({ Right(10,vw),Bottom(-26,px) }, 15000), [this]() {
		iHan->animate(Animate({ Right(10,vw),Bottom(1,px) }, 2000), [this]() {
			// simulate sleep
			iHan->animate(Animate({ Right(10,vw),Bottom(1,px) }, 3000), [this]() {
				iHan->animate(Animate({ Right(10,vw),Bottom(-26,px) }, 2000), [this]() {
					animHan();
				});
			});
		});
	});
}

void DesktopWindow::animWazzup()
{
	iWazzup->animate(Animate({ Left(47,px),Top(13,px) }, 100), [this]() {
		iWazzup->animate(Animate({ Left(42,px),Top(16, px) }, 100), [this]() {
			iWazzup->animate(Animate({ Left(45,px),Top(15,px) }, 100), [this]() {  // origin
				iWazzup->setVisible(false);
				iWazzup->getAnimate().sleep(50);

				iWazzup->animate(Animate({ Left(46,px),Top(16,px) }, 80), [this]() {
					iWazzup->setVisible(true);

					iWazzup->animate(Animate({ Left(44,px),Top(14,px) }, 80), [this]() {
						iWazzup->animate(Animate({ Left(45,px),Top(15,px) }, 80), [this]() {  // origin
							iWazzup->getAnimate().sleep(5000);
							animWazzup();
						});
					});
				});
			});
		});
	});
}

DesktopWindow::DesktopWindow(std::wstring _name, Pos _pos, Size _size)
	: Window(_name, _pos, _size, L'░')
{
	/* Elements ************************************************************/

	push_elements(iXi, Image(CharImage({ shibaRaw , {Left(22,px),Top(9,px)}, {75, 21} }), *this));
	push_elements(iWazzup, Image(CharImage({ wazzupRaw , {Left(45,px),Top(15,px)}, {121,5} }), *this));
	animWazzup();
	push_elements(iJenny, Image(CharImage({ jennyTurtleRaw , {Left(15,vw),Bottom(14,px)}, {36,15} }), *this));
	animJenny();
	push_elements(iBible, Image(CharImage({ biblethumpRaw , {Left(15,vw),Bottom(1,px)}, {33,13} }), *this));
	push_elements(iDoge, Image(CharImage({ dogeRaw , {Left(5,vw),Bottom(-10,px)}, {29,11} }), *this));
	animDoge();
	push_elements(iHan, Image(CharImage({ bloodTrailRaw, {Right(10,vw),Bottom(-26,px)}, {81,25} }), *this));
	animHan();

	push_elements(lTime, Label(L"Time", { Right(10,vw),Top(2,px) }, *this, false));
	lTime->toggleAnimateStatus();

	push_elements(bSettings, Button(L"Settings", { Left(1,px,absolute),Bottom(10,vh,absolute) }, *this, true));
	bSettings->onclick([this]() {
		System::get().createApp(AppCollection::Settings);
		return true;
	});
	bSettings->onhover([this]() {
		bSettings->isAnimatePause(bSettings->getInfo().mouseHover == ElementsInfo::active);
		return true;
	});

	push_elements(bDesktop, Button(L"Browser", { Right(3),Top(20,vh) }, *this, true));
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

	push_elements(bChess, Button(L" Crash ", { Right(3),Top(60,vh) },  *this, true));
	bChess->onhover([this]() {
		if (bChess->getInfo().mouseHover == ElementsInfo::active) bChess->setString(L" X__X ?", L'|');
		else bChess->setString(L" Crash ");
		return true;
	});
	bChess->onclick([]() {
		System::get().createApp(AppCollection::Chess);
		return true;
	});

	push_elements(bTrans, Button(L"  Toggle Devtool Transparency  ", { Left(5,px,relative),Top(4,px,relative) },  *this, false, {3,1}));
	bTrans->onclick([this]() {
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

		std::wstring str = std::to_wstring(t.tm_year + 1900) + TRANSPARENT_WCHAR + std::to_wstring(t.tm_mon + 1) + L'/' + std::to_wstring(t.tm_mday) + TRANSPARENT_WCHAR +
			(t.tm_hour < 10 ? L"0" : L"\0") + std::to_wstring(t.tm_hour) + L':' +
			(t.tm_min < 10 ? L"0" : L"\0") + std::to_wstring(t.tm_min) + L':' +
			(t.tm_sec < 10 ? L"0" : L"\0") + std::to_wstring(t.tm_sec) +
			(int(i*MY_UPDATE_PERIOD) / 10 < 10 ? L":0" : L":") + std::to_wstring(int(i*MY_UPDATE_PERIOD) / 10);

		lTime->setString(str);
		return Status::refresh;

		// experimental
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