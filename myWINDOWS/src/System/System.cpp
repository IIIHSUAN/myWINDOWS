#include <thread>
#include <algorithm>

#include "System.h"

#include "App/App.h"
#include "App/Collection/Desktop/Desktop.h"
#include "App/Collection/Settings/Settings.h"
#include "App/Collection/Painter/Painter.h"
#include "App/Collection/Chess/Chess.h"


System* System::appHandler = new System();

System::System()
{
	eventCallback = std::bind(&System::onEvent, this, std::placeholders::_1);

	return;

	/* opening *******************************************/
	// use canvas draw
	auto& photo = img;
	canvas.flush(L'⣿');
	for (int i = 0; i < photo.getRowCount(); i++)
		canvas.getRaw().replace((i + MY_WINDOW_HEIGHT / 2 - photo.getRowCount() / 2)*MY_WINDOW_WIDTH + MY_WINDOW_WIDTH / 2 - photo.getColumnCount() / 2, photo.getColumnCount(), photo.getData(), i*(photo.getColumnCount() - 1), photo.getColumnCount());
	canvas.lineCenter(MY_WINDOW_WIDTH - 20, MY_WINDOW_HEIGHT / 2, std::wstring(L"  Welcome back !  "));
	update(0);
	Sleep(500);

	photo = img2;
	canvas.flush(L'⣿');
	for (int i = 0; i < photo.getRowCount(); i++)
		canvas.getRaw().replace((i + MY_WINDOW_HEIGHT / 2 - photo.getRowCount() / 2)*MY_WINDOW_WIDTH + MY_WINDOW_WIDTH / 2 - photo.getColumnCount() / 2, photo.getColumnCount(), photo.getData(), i*(photo.getColumnCount() - 1), photo.getColumnCount());
	canvas.lineCenter(MY_WINDOW_WIDTH - 20, MY_WINDOW_HEIGHT / 2, std::wstring(L"  Welcome back !  "));
	update(0);
	Sleep(500);
	photo = img3;
	canvas.flush(L'⣿');
	for (int i = 0; i < photo.getRowCount(); i++)
		canvas.getRaw().replace((i + MY_WINDOW_HEIGHT / 2 - photo.getRowCount() / 2)*MY_WINDOW_WIDTH + MY_WINDOW_WIDTH / 2 - photo.getColumnCount() / 2, photo.getColumnCount(), photo.getData(), i*(photo.getColumnCount() - 1), photo.getColumnCount());
	canvas.lineCenter(MY_WINDOW_WIDTH - 20, MY_WINDOW_HEIGHT / 2, std::wstring(L"  Welcome back !  "));
	update(0);
	Sleep(500);
	photo = img4;
	canvas.flush(L'⣿');
	for (int i = 0; i < photo.getRowCount(); i++)
		canvas.getRaw().replace((i + MY_WINDOW_HEIGHT / 2 - photo.getRowCount() / 2)*MY_WINDOW_WIDTH + MY_WINDOW_WIDTH / 2 - photo.getColumnCount() / 2, photo.getColumnCount(), photo.getData(), i*(photo.getColumnCount() - 1), photo.getColumnCount());
	canvas.lineCenter(MY_WINDOW_WIDTH - 20, MY_WINDOW_HEIGHT / 2, std::wstring(L"  Welcome back !  "));
	update(0);
	Sleep(500);
}

void System::run()
{
	createApp(AppCollection::Desktop);
	//createApp(AppCollection::Painter);

	pollingThread = std::thread([this]() {try { pollingUpdate(); } catch (...) { return; } });
	inputThread = std::thread([]() {	try { Input::get().run(); } catch (...) { return; }});

	pollingThread.join();
	inputThread.join();
}

void System::createApp(AppCollection name)
{
	static Pos windowPos = { 0,0 };
	windowPos++;

	switch (name)
	{
	default:
	case AppCollection::unknown:
		return;
	case AppCollection::Desktop:
		appList.emplace_back(new Desktop(windowPos));
		break;
	case AppCollection::Menu:
		appList.emplace_back(new Desktop(windowPos));
		break;
	case AppCollection::WindowManager:
		appList.emplace_back(new Desktop(windowPos));
		break;
	case AppCollection::Settings:
		appList.emplace_back(new Settings(windowPos));
		break;
	case AppCollection::Painter:
		appList.emplace_back(new Painter(windowPos));
		break;
	case AppCollection::MyPhoto:
		appList.emplace_back(new Desktop(windowPos));
		break;
	case AppCollection::Chess:
		appList.emplace_back(new Chess(windowPos));
		break;
	}

	runThreadList.emplace_back(std::thread([this]() {
		try {
			appList.back()->run();  // if need update UI, use PollingUpdate
		}
		catch (...)
		{
			// msg here
			return;
		}
	}));
	runThreadList.back().join();
}

void System::shutdown()
{
	isRun = false;

	if (!appList.empty())
		onEvent(ShutdownEvent());

	return;

	// use canvas draw
	auto& photo = img3;
	canvas.flush(L'⣿');
	for (int i = 0; i < photo.getRowCount(); i++)
		canvas.getRaw().replace((i + MY_WINDOW_HEIGHT / 2 - photo.getRowCount() / 2)*MY_WINDOW_WIDTH + MY_WINDOW_WIDTH / 2 - photo.getColumnCount() / 2, photo.getColumnCount(), photo.getData(), i*(photo.getColumnCount() - 1), photo.getColumnCount());
	canvas.lineCenter(MY_WINDOW_WIDTH - 20, MY_WINDOW_HEIGHT / 2, std::wstring(L"  Bye!  "));
	update(0);
	Sleep(5000);
}

void System::pollingUpdate()
{
	bool isNeedUpdate;

	while (isRun)
	{
		time = clock();

		isNeedUpdate = false;
		for (auto* app : appList)
			if (app->pollingUpdate())
				isNeedUpdate = true;

		if (!Input::get().getIsEventUpdate() || isNeedUpdate)
			update();

		Sleep(int(1000 * pollingPeriod)), fps = 1000.0f / (clock() - time);
	}
}

void System::update(bool isFlush)
{
	static bool isUpdating;
	if (isUpdating)
		return;
	else
		isUpdating = true;

	if (isFlush)
		canvas.flush();

	for (App* app : appList)
		for (auto& window : app->getWindowList())
			canvas.renderWindow(*window);

	// msgThread
	if (isMsg)
	{
		canvas.renderWith(msgCanvas);
		if (!isMsgRun)
		{
			isMsgRun = true;
			msgThread = std::thread([this]() {
				try {
					while (--isMsg) Sleep(500); isMsgRun = false, update();
				} catch (...) { return; }
			});
			msgThread.detach();
		}
	}

	// mouseUI
	canvas.getRaw().at(index(Mouse::get().Y, Mouse::get().X)) = (Mouse::get().isPrs ? L'\u29C8' : L'\u25A2');

	// draw
	Output::get().display(canvas.getRawConst());

	isUpdating = false;
}

void System::onEvent(Event & e)  // from input
{
	if (e.getType() == Event::shutdown)
		for (auto& app : appList)
			app->onEvent(e);

	if (e.getType() != Event::unknown)
		for (auto app = appList.rbegin(); app != appList.rend();)
		{
			if ((*app)->onEvent(e))
			{
				// window on focus
				if (e.getType() == Event::mousePrs && *appList.rbegin() != *app)
				{
					auto it = std::next(appList.begin(), std::distance(appList.begin(), app.base()) - 1);
					appList.splice(it, appList, std::next(it), appList.end());
				}

				break;
			}

			if (!(*app)->getIsRun())
			{
				delete (*app);
				app = std::list<App*>::reverse_iterator(appList.erase(--app.base()));
			}
			else
				app++;
		}

	if (appList.empty())
		shutdown();
}

bool System::keyEvent(WORD key, DWORD ctrl, bool isPrs)
{
	if (!isPrs)
		return false;

	Event::Type et = Event::unknown;
	Mouse& mouse = Mouse::get();

	if ((ctrl & RIGHT_CTRL_PRESSED)||(ctrl & SHIFT_PRESSED))
	{
		msgStr = L"  Mouse speed: ";
		mouse.changeSpeed(), msgStr += mouse.speed == Mouse::normal ? L"normal  " : mouse.speed == Mouse::fast ? L"fast  " : L"very fast  ";
		msgCanvas.canvasCenterLine(msgStr);
		isMsg = 3;
	}

	mouse.offsetX = 0, mouse.offsetY = 0;
	switch (KeyPrsEvent::toKeySet(key))
	{
	case KeySet::esc:
		shutdown();
		break;
	case KeySet::up:
		if (mouse.Y <= 0) return false;

		et = Event::mouseMove;
		mouse.offsetY = -mouse.speed;
		mouse.Y = max(mouse.Y + mouse.offsetY, 0);
		break;
	case KeySet::down:
		if (mouse.Y >= MY_WINDOW_HEIGHT - 1) return false;

		et = Event::mouseMove;
		mouse.offsetY = mouse.speed;
		mouse.Y = min(mouse.Y + mouse.offsetY, MY_WINDOW_HEIGHT - 1);
		break;
	case KeySet::left:
		if (mouse.X <= 0) return false;

		et = Event::mouseMove;
		mouse.offsetX = -mouse.speed;
		mouse.X = max(mouse.X + mouse.offsetX, 0);
		break;
	case KeySet::right:
		if (mouse.X >= MY_WINDOW_WIDTH - 1) return false;

		et = Event::mouseMove;
		mouse.offsetX = mouse.speed;
		mouse.X = min(mouse.X + mouse.offsetX, MY_WINDOW_WIDTH - 1);
		break;
	case KeySet::spacebar:  // VK_SPACE
		mouse.isPrs = !mouse.isPrs;
		et = mouse.isPrs ? Event::mousePrs : Event::mouseRls;
		break;
	default:
		et = Event::keyPrs;
		break;
	}

	switch (et)
	{
	case Event::mouseMove:
	{
		MouseMoveEvent e(mouse.X - mouse.offsetX, mouse.Y - mouse.offsetY, mouse.offsetX, mouse.offsetY, mouse.isPrs);
		eventCallback(e);
		break;
	}
	case Event::mousePrs:
	{
		MousePrsEvent e(mouse.X, mouse.Y, key);
		eventCallback(e);
		break;
	}
	case Event::mouseRls:
	{
		MouseRlsEvent e(mouse.X, mouse.Y, key);
		eventCallback(e);
		break;
	}
	case Event::keyPrs:
	{
		KeyPrsEvent e(key);
		eventCallback(e);
		break;
	}
	}

	return true;
}
