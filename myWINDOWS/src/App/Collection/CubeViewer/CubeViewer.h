#pragma once

#include "App/App.h"

class CubeViewerWindow :public Window
{
public:
	CubeViewerWindow() :Window(std::wstring(L"CubeViewer"), { 0,0 }, { MY_WINDOW_WIDTH,MY_WINDOW_HEIGHT }, L'.') {}
private:

};

class CubeViewer :public App
{
public:
	CubeViewer(Pos pos = { 3,1 }, UINT id = 0) : App(AppCollection::CubeViewer, id) {
		push_window(cubeViewerWindow, CubeViewerWindow());

		push_window(devToolWindow, DevToolWindow({ MY_WINDOW_WIDTH - 40, 2 }, { 40,20 }, false));
		devTool_push(devToolWindow, System::get().getFps());
	}
private:
	std::shared_ptr<Window> cubeViewerWindow, devToolWindow;
};