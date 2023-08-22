#include <algorithm>

#include "System.h"

#include "App/Collection/AppCollection.h"
#include "App/App.h"

/*
 *  Events vs. PollingUpdate: 
 *    [Previous Version] Events (able block handler) & PollingUpdate both can refresh canvas
 *    [Present Version]  Events (able block handler) set PollingStatus & push to queue, 
 *                       PollingUpdate pop it & canvas: update / allRefresh & if(!isRun) erase;
 */

System* System::appHandler = new System();

void System::run()
{
	//createApp(AppCollection::CubeViewer);
	createApp(AppCollection::Desktop);

	pollingThread = std::thread([this]() {try { pollingUpdate(); } catch (...) { __debugbreak(); } });
	inputThread = std::thread([]() { try { Input::get().run(); } catch (...) { __debugbreak(); }});

	//pollingThread.join();
	inputThread.join();
}

void System::createApp(AppCollection name)
{
	static UINT id = -1;
	id++;
	appStartPos++;

	switch (name)
	{
	default:
	case AppCollection::unknown:
		return;
	case AppCollection::Desktop:
		appList.emplace_back(new Desktop(appStartPos, id));
		break;
	case AppCollection::Settings:
		appList.emplace_back(new Settings(appStartPos, id));
		break;
	case AppCollection::Painter:
		appList.emplace_back(new Painter(appStartPos, id));
		break;
	case AppCollection::Chess:
		appList.emplace_back(new Chess(appStartPos, id));
		break;
	case AppCollection::CubeViewer:
		appList.emplace_back(new CubeViewer(appStartPos, id));
		break;
	}

	runThreadList.emplace_back(std::thread([this]() {
		App* latestApp = appList.back();
		try {
			latestApp->run();  // if need update UI, use PollingUpdate
		}
		catch (...)
		{
			// msg here

			latestApp->closeApp();
			return;
		}
	}));
	runThreadList.back().detach();
}

void System::pollingUpdate()
{
	static Status status;
	static std::list<App*>::iterator baseUpdateApp;

	while (isRun)
	{
		time = clock();

		while (!eventQueue.empty())
		{
			Event* e = eventQueue.front();
			eventQueue.pop();
			onEvent(*e);
			delete (Event*)e;
		}

		baseUpdateApp = appList.end();
		for (auto app = appList.begin(); app != appList.end();)  // reset all polling Status
		{
			status = (*app)->pollingUpdate();

			if (status == Status::refresh)  // window resize only
				isForceRefresh |= true;
			else if (status == Status::needUpdate && baseUpdateApp == appList.end())
				baseUpdateApp = app;

			if (!(*app)->getIsRun())
			{
				delete (App*)(*app);
				app = appList.erase(app);
			}
			else
				app++;
		}

		if (isForceRefresh || baseUpdateApp != appList.end())
			update(baseUpdateApp);

		if (isPollingPeriod)
			Sleep(pollingPeriod);
		fps = 1000.0f / (clock() - time);
	}
}

void System::update(std::list<App*>::iterator _app)
{
	if (isForceRefresh)
		canvas.flush(), _app = appList.begin(), isForceRefresh = false;

	for (auto app = _app; app != appList.end(); app++)  // render from top to _app
		for (const auto& window : (*app)->getWindowList())
			canvas.renderWindow(*window);

	// msgThread
	if (msgCount)
	{
		canvas.renderWith(msgCanvas);
		if (!isMsgRun)
		{
			isMsgRun = true;
			msgThread = std::thread([this]() {
				while (--msgCount) Sleep(500);
				isMsgRun = false, isForceRefresh = true;
			});
			msgThread.detach();
		}
	}

	// mouseUI
	canvas.getRaw()[index(MyMouse::get().Y, MyMouse::get().X)] = (MyMouse::get().isPrs ? L'\u29C8' : L'\u25A2');

	// draw
	Output::get().display(canvas.getRawConst());
}

void System::onEvent(Event& e)  // from input
{
	if (e.getType() == Event::shutDown)
		for (auto& app : appList)
			app->onEvent(e);
	else if (e.getType() == Event::resize)
	{
		ResizeEvent& re = (ResizeEvent&)e;
		Output::get().onResizeEvent(re);
		msgCanvas.resizeRawData(), canvas.resizeRawData(), canvas.setSize({ MY_WINDOW_WIDTH,MY_WINDOW_HEIGHT });
		
		isForceRefresh = true;
		wprintf(L"\n");
	}

	if (e.getType() != Event::unknown)
		for (auto& app = appList.rbegin(); app != appList.rend(); app++)
		{
			if ((*app)->onEvent(e))
			{
				if (e.getType() == Event::mousePrs && app != appList.rbegin())  // window on focus
				{
					auto it = std::next(appList.begin(), std::distance(appList.begin(), app.base()) - 1);
					appList.splice(it, appList, std::next(it), appList.end());
				}

				break;
			}

			if (app == std::next(appList.rend(), -1) && e.getType() == Event::mouseMove)  // mouseMove && no one handled
				isForceRefresh |= true;
		}

	if (appList.empty())
		shutdown();
}

// from IO
void System::onResizeEvent(short width, short height)
{
	auto *e = new ResizeEvent(width, height, float(width) / MY_WINDOW_WIDTH, float(height) / MY_WINDOW_HEIGHT);
	eventQueue.emplace(e);
}
void System::onKeyPrsEvent(unsigned short key, WCHAR unicodeChar)
{
	Event::Type et = Event::unknown;
	MyMouse& mouse = MyMouse::get();

	mouse.offsetX = 0, mouse.offsetY = 0;
	switch (KeyPrsEvent::toKeySet(key))
	{
	case KeySet::esc:
		shutdown();
		break;
	case KeySet::up:
		if (mouse.Y <= 0) return;

		et = Event::mouseMove;
		mouse.offsetY = -mouse.speed;
		mouse.Y = max(mouse.Y + mouse.offsetY, 0);
		break;
	case KeySet::down:
		if (mouse.Y >= MY_WINDOW_HEIGHT - 1) return;

		et = Event::mouseMove;
		mouse.offsetY = mouse.speed;
		mouse.Y = min(mouse.Y + mouse.offsetY, MY_WINDOW_HEIGHT - 1);
		break;
	case KeySet::left:
		if (mouse.X <= 0) return;

		et = Event::mouseMove;
		mouse.offsetX = -mouse.speed;
		mouse.X = max(mouse.X + mouse.offsetX, 0);
		break;
	case KeySet::right:
		if (mouse.X >= MY_WINDOW_WIDTH - 1) return;

		et = Event::mouseMove;
		mouse.offsetX = mouse.speed;
		mouse.X = min(mouse.X + mouse.offsetX, MY_WINDOW_WIDTH - 1);
		break;
	case KeySet::spacebar:
		mouse.isPrs = !mouse.isPrs;
		et = mouse.isPrs ? Event::mousePrs : Event::mouseRls;
		break;
	case KeySet::alt:
		msgStr = L"  Mouse speed: ";
		mouse.changeSpeed(), msgStr += mouse.speed == MyMouse::normal ? L"normal  " : mouse.speed == MyMouse::fast ? L"fast  " : L"very fast  ";
		msgCanvas.canvasCenterLine(msgStr);
		msgCount = 3;
		break;
	case KeySet::shift:
		break;
	default:
		et = Event::keyPrs;
		break;
	}

	switch (et)
	{
	case Event::mouseMove:
	{
		auto *e = new MouseMoveEvent(mouse.X - mouse.offsetX, mouse.Y - mouse.offsetY, mouse.offsetX, mouse.offsetY, mouse.isPrs);
		eventQueue.emplace(e);
		break;
	}
	case Event::mousePrs:
	{
		auto *e = new MousePrsEvent(mouse.X, mouse.Y, PollingInput::mouse_l1);
		eventQueue.emplace(e);
		break;
	}
	case Event::mouseRls:
	{
		auto *e = new MouseRlsEvent(mouse.X, mouse.Y);
		eventQueue.emplace(e);
		break;
	}
	case Event::keyPrs:
	{
		auto *e = new KeyPrsEvent(key, unicodeChar);
		eventQueue.emplace(e);
		break;
	}
	}
}
void System::onMouseMoveEvent(int X, int Y)
{
	if (Y < 0 || Y > MY_WINDOW_HEIGHT || X < 0 || X > MY_WINDOW_WIDTH)
		return;
	MyMouse& mouse = MyMouse::get();

	mouse.offsetX = X - mouse.X, mouse.offsetY = Y - mouse.Y;
	if (mouse.offsetX || mouse.offsetY)
	{
		auto *e = new MouseMoveEvent(mouse.X, mouse.Y, mouse.offsetX, mouse.offsetY, mouse.isPrs);
		eventQueue.emplace(e);
		mouse.X = X, mouse.Y = Y;
	}
}
void System::onMousePrsEvent(PollingInput input)
{
	MyMouse& mouse = MyMouse::get();
	if (input == PollingInput::mouse_l1) 
		mouse.isPrs = true;

	auto *e = new MousePrsEvent(mouse.X, mouse.Y, input);
	eventQueue.emplace(e);
}
void System::onMouseRlsEvent()
{
	MyMouse& mouse = MyMouse::get();
	if (mouse.isPrs)
	{
		mouse.isPrs = false;

		auto *e = new MouseRlsEvent(mouse.X, mouse.Y);
		eventQueue.emplace(e);
	}
}

void System::shutdown()
{
	isRun = false;

	if (!appList.empty())
		onEvent(ShutdownEvent());

	return;

	//// use canvas draw
	//auto& photo = img3;
	//canvas.flush(L'⣿');
	//for (int i = 0; i < photo.getRowCount(); i++)
	//	canvas.getRaw().replace((i + MY_WINDOW_HEIGHT / 2 - photo.getRowCount() / 2)*MY_WINDOW_WIDTH + MY_WINDOW_WIDTH / 2 - photo.getColumnCount() / 2, photo.getColumnCount(), photo.getData(), i*(photo.getColumnCount() - 1), photo.getColumnCount());
	//canvas.lineCenter(MY_WINDOW_WIDTH - 20, MY_WINDOW_HEIGHT / 2, std::wstring(L"  Bye!  "));
	//update(0);
	//Sleep(5000);
}