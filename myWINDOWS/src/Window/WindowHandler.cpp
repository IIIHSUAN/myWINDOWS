﻿#include <iostream>
#include <thread>

#include "WindowHandler.h"

#include "../App/Collection/Desktop.h"
#include "../App/Collection/Painter.h"


WindowHandler* WindowHandler::windowHandler = new WindowHandler();

WindowHandler::WindowHandler()
{
	eventCallback = std::bind(&WindowHandler::onEvent, this, std::placeholders::_1);

	return;

	/* opening *******************************************/
	// use canvas draw
	auto& photo = img;
	canvas.flush(L'⣿');
	for (int i = 0; i < photo.getRowCount(); i++)
		canvas.getCanvas().replace((i + MY_WINDOW_HEIGHT / 2 - photo.getRowCount() / 2)*MY_WINDOW_WIDTH + MY_WINDOW_WIDTH / 2 - photo.getColumnCount() / 2, photo.getColumnCount(), photo.getData(), i*(photo.getColumnCount() - 1), photo.getColumnCount());
	canvas.getCanvas().replace(1572, 18, L"  Welcome Back!  ");
	update(0);
	Sleep(1000);
}

void WindowHandler::run()
{
	createApp(AppCollection::Desktop);
	//createApp(AppCollection::Painter);

	std::thread pollingThread(&WindowHandler::pollingUpdate, this);
	std::thread inputThread([]() {	Input::get().run(); });

	pollingThread.detach();
	inputThread.join();
}

void WindowHandler::createApp(AppCollection name)
{
	switch (name)
	{
	default:
	case AppCollection::Unknown:
		return;
	case AppCollection::Desktop:
		appVec.emplace_back(new Desktop());
		break;
	case AppCollection::Menu:
		appVec.emplace_back(new Desktop());
		break;
	case AppCollection::WindowManager:
		appVec.emplace_back(new Desktop());
		break;
	case AppCollection::Setting:
		appVec.emplace_back(new Desktop());
		break;
	case AppCollection::Painter:
		appVec.emplace_back(new Painter());
		break;
	case AppCollection::MyPhoto:
		appVec.emplace_back(new Desktop());
		break;
	case AppCollection::Chess:
		appVec.emplace_back(new Desktop());
		break;
	}

	std::thread t([this]() { appVec[appVec.size() - 1]->run(); });
	t.detach();
}

void WindowHandler::pollingUpdate()
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

		Sleep(1000*pollingPeriod);
	}
}

void WindowHandler::update(bool isFlush)
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
	canvas.getCanvas().at(index(mouse.Y, mouse.X)) = (mouse.isPressed ? L'\u29C8' : L'\u25A2');

	// draw
	Output::get().display(canvas.getConstCanvas()); 

	isUpdating = false;
}

void WindowHandler::onEvent(Event & e)  // from input
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
}

bool WindowHandler::keyEvent(WORD key, DWORD ctrl, bool isPressed)
{
	if (!isPressed)
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
		isRun = false;
		break;
	case 0x57:
	case VK_UP:
		e = EventType::mouseMove;
		mouse.offsetY = -mouse.speed;
		mouse.Y = max(mouse.Y + mouse.offsetY, 0);
		break;
	case 0x53:
	case VK_DOWN:
		e = EventType::mouseMove;
		mouse.offsetY = mouse.speed;
		mouse.Y = min(mouse.Y + mouse.offsetY, MY_WINDOW_HEIGHT - 1);
		break;
	case 0x41:
	case VK_LEFT:
		e = EventType::mouseMove;
		mouse.offsetX = -mouse.speed;
		mouse.X = max(mouse.X + mouse.offsetX, 0);
		break;
	case 0x44:
	case VK_RIGHT:
		e = EventType::mouseMove;
		mouse.offsetX = mouse.speed;
		mouse.X = min(mouse.X + mouse.offsetX, MY_WINDOW_WIDTH - 1);
		break;
	case 0x20:
		mouse.isPressed = !mouse.isPressed;
		e = mouse.isPressed ? EventType::mousePrs : EventType::mouseRls;
		break;
	}

	switch (e)
	{
	case EventType::mouseMove:
	{
		MouseMoveEvent event(mouse.X, mouse.Y, mouse.offsetX, mouse.offsetY, mouse.isPressed);

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
