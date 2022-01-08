#include <iostream>
#include <thread>

#include "AppHandler.h"

#include "../App/Collection/Desktop/Desktop.h"
#include "../App/Collection/Settings/Settings.h"
#include "../App/Collection/Painter/Painter.h"
#include "../App/Collection/Chess/Chess.h"


AppHandler* AppHandler::appHandler = new AppHandler();

AppHandler::AppHandler()
{
	eventCallback = std::bind(&AppHandler::onEvent, this, std::placeholders::_1);

	return;

	/* opening *******************************************/
	// use canvas draw
	auto& photo = img;
	canvas.flush(L'⣿');
	for (int i = 0; i < photo.getRowCount(); i++)
		canvas.getCanvas().replace((i + MY_WINDOW_HEIGHT / 2 - photo.getRowCount() / 2)*MY_WINDOW_WIDTH + MY_WINDOW_WIDTH / 2 - photo.getColumnCount() / 2, photo.getColumnCount(), photo.getData(), i*(photo.getColumnCount() - 1), photo.getColumnCount());
	canvas.lineCenter(MY_WINDOW_WIDTH - 20, MY_WINDOW_HEIGHT / 2, std::wstring(L"  Welcome back !  "));
	update(0);
	Sleep(1000);
}

void AppHandler::run()
{
	//createApp(AppCollection::Desktop);
	createApp(AppCollection::Chess);

	std::thread pollingThread(&AppHandler::pollingUpdate, this);
	std::thread inputThread([]() {	Input::get().run(); });

	pollingThread.detach();
	inputThread.join();
}

void AppHandler::createApp(AppCollection name)
{
	static Pos windowPos = { 0,0 };

	windowPos++;

	switch (name)
	{
	default:
	case AppCollection::Unknown:
		return;
	case AppCollection::Desktop:
		appVec.emplace_back(new Desktop(windowPos));
		break;
	case AppCollection::Menu:
		appVec.emplace_back(new Desktop(windowPos));
		break;
	case AppCollection::WindowManager:
		appVec.emplace_back(new Desktop(windowPos));
		break;
	case AppCollection::Settings:
		appVec.emplace_back(new Settings(windowPos));
		break;
	case AppCollection::Painter:
		appVec.emplace_back(new Painter(windowPos));
		break;
	case AppCollection::MyPhoto:
		appVec.emplace_back(new Desktop(windowPos));
		break;
	case AppCollection::Chess:
		appVec.emplace_back(new Chess(windowPos));
		break;
	}

	std::thread t([this]() { appVec[appVec.size() - 1]->run(); });
	t.detach();
}

void AppHandler::shutdown()
{
	isRun = false;

	// use canvas draw
	auto& photo = img;
	canvas.flush(L'⣿');
	for (int i = 0; i < photo.getRowCount(); i++)
		canvas.getCanvas().replace((i + MY_WINDOW_HEIGHT / 2 - photo.getRowCount() / 2)*MY_WINDOW_WIDTH + MY_WINDOW_WIDTH / 2 - photo.getColumnCount() / 2, photo.getColumnCount(), photo.getData(), i*(photo.getColumnCount() - 1), photo.getColumnCount());
	canvas.lineCenter(MY_WINDOW_WIDTH - 20, MY_WINDOW_HEIGHT / 2, std::wstring(L"  Bye!  "));
	update(0);

	//Sleep(2000);
}

void AppHandler::pollingUpdate()
{
	bool isNeedUpdate;
	while (isRun)
	{
		isNeedUpdate = false;
		for (auto* app : appVec)
			if (app->pollingUpdate())
				isNeedUpdate = true;

		if (isNeedUpdate)
			update();

		Sleep(1000 * pollingPeriod);
	}
}

void AppHandler::update(bool isFlush)
{
	static bool isUpdating;
	if (isUpdating)
		return;
	else
		isUpdating = true;

	if (isFlush)
		canvas.flush();

	for (App* app : appVec)
		for (auto& window : app->getWindowVec())
			canvas.renderWindow(*window);

	// msgThread
	if (isMsg)
	{
		canvas.renderWithRel(msgCanvas);
		if (!isMsgRun)
		{
			isMsgRun = true;
			std::thread t([this]() { while (--isMsg) Sleep(500); isMsgRun = false, update(); });
			t.detach();
		}
	}
	
	// mouseUI
	canvas.getCanvas().at(index(mouse.Y, mouse.X)) = (mouse.isPrs ? L'\u29C8' : L'\u25A2');

	// draw
	Output::get().display(canvas.getConstCanvas()); 

	isUpdating = false;
}

void AppHandler::onEvent(Event & e)  // from input
{
	if (e.getType() != EventType::unknown)
		for (auto app = appVec.rbegin(); app != appVec.rend();)
		{
			if ((*app)->onEvent(e))
				break;

			if (!(*app)->getIsRun())
			{
				delete (*app);
				auto it = appVec.erase(--app.base());
				app = std::vector<App*>::reverse_iterator(it);
			}
			else
				app++;
		}

	if (appVec.empty())
		shutdown();
}

bool AppHandler::keyEvent(WORD key, DWORD ctrl, bool isPrs)
{
	if (!isPrs)
		return false;

	EventType e = EventType::unknown;

	if ((ctrl & RIGHT_CTRL_PRESSED)||(ctrl & SHIFT_PRESSED))
	{
		msgStr = L"  Mouse speed: ";
		mouse.changeSpeed(), msgStr += mouse.speed == Mouse::normal ? L"normal  " : mouse.speed == Mouse::fast ? L"fast  " : L"very fast  ";
		msgCanvas.canvasCenterLine(msgStr);
		isMsg = 3;
	}

	mouse.offsetX = 0, mouse.offsetY = 0;
	switch (key)
	{
	case VK_ESCAPE:
		shutdown();
		break;
	case VK_UP:
		e = EventType::mouseMove;
		mouse.offsetY = -mouse.speed;
		mouse.Y = max(mouse.Y + mouse.offsetY, 0);
		break;
	case VK_DOWN:
		e = EventType::mouseMove;
		mouse.offsetY = mouse.speed;
		mouse.Y = min(mouse.Y + mouse.offsetY, MY_WINDOW_HEIGHT - 1);
		break;
	case VK_LEFT:
		e = EventType::mouseMove;
		mouse.offsetX = -mouse.speed;
		mouse.X = max(mouse.X + mouse.offsetX, 0);
		break;
	case VK_RIGHT:
		e = EventType::mouseMove;
		mouse.offsetX = mouse.speed;
		mouse.X = min(mouse.X + mouse.offsetX, MY_WINDOW_WIDTH - 1);
		break;
	case 0x20:  // VK_SPACE
		mouse.isPrs = !mouse.isPrs;
		e = mouse.isPrs ? EventType::mousePrs : EventType::mouseRls;
		break;
	default:
		e = EventType::keyPrs;
		break;
	}

	

	switch (e)
	{
	case EventType::mouseMove:
	{
		MouseMoveEvent event(mouse.X, mouse.Y, mouse.offsetX, mouse.offsetY, mouse.isPrs);

		// out of bound
		if ((event.getMouseX() == 0 && event.getOffsetX() < -1) || (event.getMouseX() == MY_WINDOW_WIDTH && event.getOffsetX() > 0))
			event.setOffsetX(0);
		if ((event.getMouseY() == 0 && event.getOffsetY() < -1) || (event.getMouseY() == MY_WINDOW_HEIGHT && event.getOffsetY() > 0))
			event.setOffsetY(0);

		eventCallback(event);
		break;
	}
	case EventType::mousePrs:
	{
		MousePrsEvent event(mouse.X, mouse.Y, key);
		eventCallback(event);
		break;
	}
	case EventType::mouseRls:
	{
		MouseRlsEvent event(mouse.X, mouse.Y, key);
		eventCallback(event);
		break;
	}
	case EventType::keyPrs:
	{
		KeyPrsEvent event(key);
		eventCallback(event);
		break;
	}
	}

	return true;
}
