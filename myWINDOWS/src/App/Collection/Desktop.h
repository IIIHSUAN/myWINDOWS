#pragma once

#include "../App.h"

#include <ctime>

class Desktop;

class DesktopWindow :public Window
{
public:
	DesktopWindow(App& app, int _id, std::wstring _name, Pos _pos, Size _size);
private:
	std::shared_ptr<Button>bPainter, bChess;

	const wchar_t* backgroundData = L"\
****************************************************\
***                     ****************************\
****************************************************\
****************************************************\
****************************************************\
****************************************************\
****************************************************";
	CharImage background = { backgroundData ,{10,5}, {53,7} };
};

class Desktop :public App
{
public:
	Desktop() : App(AppCollection::Desktop, windowVec) {
		CREATE_WINDOW(windowVec, DesktopWindow(*this, windowIdNum++, L"ELEMENTS", { 5,2 }, { MY_WINDOW_WIDTH - 5,MY_WINDOW_HEIGHT - 10 }));

		//PUSH_WINDOW(mainWindow, DesktopWindow(*this, 0, L"ELEMENTS", { 5,2 }, { MY_WINDOW_WIDTH - 5,MY_WINDOW_HEIGHT - 10 }));
		//CREATE_WINDOW(DesktopWindow(this, 0, L"ELEMENTS", { 5,2 }, { MY_WINDOW_WIDTH - 5,MY_WINDOW_HEIGHT - 10 }));
	}
	void a() {  }
	virtual void run() override;
private:
	std::vector<std::shared_ptr<Window>> windowVec;
	unsigned int windowIdNum = 0;
};