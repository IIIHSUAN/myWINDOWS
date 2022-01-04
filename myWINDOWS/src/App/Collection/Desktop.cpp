#include "Desktop.h"

#include <Windows.h>

#include "../../Window/WindowHandler.h"

DesktopWindow::DesktopWindow(App& app, int _id, std::wstring _name, Pos _pos, Size _size)
	: Window(app, _id, _name, _pos, _size)
{
	Window::getCanvas().setBackground(background);

	//PUSH_ELEMENTS(bPainter, Button(L"  Painter  ", { 70, 5 }, true));

	//PUSH_ELEMENTS(bChess, Button(L"   Chess   ", { 70, 10 }, true));


	//app.create(DesktopWindow(app, 0, L"Sub Desktop", { 12,6 }, { MY_WINDOW_WIDTH,MY_WINDOW_HEIGHT }));

	//Button& bb = elementsVec[0]->get<Button>();
	//bb.onHover([&bb]() {
	//	bb.setString(L" Open Painter ? ", L'|');
	//	return false;
	//});
	//bb.onPressed([&bb]() {
	//	bb.setString(L"  Painter  ");
	//	return false;
	//});
	//bb.onclick([&app]() {
	//	app.create(DesktopWindow(app, 0, L"Sub Desktop", { 12,6 }, { MY_WINDOW_WIDTH,MY_WINDOW_HEIGHT }));

	//	//WindowHandler::get().getAppVec()[0]->create();
	//	return false;
	//});

	//Window::createElement<Button>(Button(L"   Chess   ", { 70, 10 }, true));
	//Button& bbb = elementsVec[1]->get<Button>();
	//bbb.onHover([&bbb]() {
	//	bbb.setString(L" Play ", L'*');
	//	return false;
	//});
	//bbb.onPressed([&bbb]() {
	//	bbb.setString(L"  Chess  ");
	//	return false;
	//});
	//bbb.onclick([&bbb]() {
	//	//WindowHandler::get().getAppVec()[0]->create(std::make_shared<DesktopWindow>(DesktopWindow(0, L"Painter", { 12,6 }, { MY_WINDOW_WIDTH,MY_WINDOW_HEIGHT })));
	//	return false;
	//});

	Window::update();
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

		windowVec[0]->getCanvas().line(15, 6, str.c_str(), str.length()), App::setPollingUpdate(true), Sleep(1000);
	}
}