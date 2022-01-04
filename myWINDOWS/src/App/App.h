#pragma once

#include <vector>

#include "Window/Window.h"
#include "AppCollection.h"

class AppHandler;

#define CREATE_WINDOW(windowVec, windowEntity) windowVec.emplace_back(std::make_shared<Window>(windowEntity));

/*
 * template
 * public get App::member via public:
 * child using App::member via App::func()
 */

class App
{
public:
	App(AppCollection app, std::vector<std::shared_ptr<Window>>& _windowVec) : app(app), windowVec(_windowVec) {}

	friend AppHandler;

	inline void setWindowVec(std::vector<std::shared_ptr<Window>>& _windowVec) { windowVec = _windowVec; }
	inline std::vector<std::shared_ptr<Window>>& getWindowVec() { return windowVec; }

	inline AppCollection& getApp() { return app; }
	inline bool& getIsRun() { return isRun; }

	bool pollingUpdate();
	inline void setPollingUpdate(bool b) { isNeedUpdate = b; }

	virtual void run() {}

	inline void pushWindow(std::shared_ptr<Window>& window) { windowVec.emplace_back(window); }
private:
	AppCollection app;
	Pos subWindowPos = { 0,0 };
	bool isRun = true, isNeedUpdate = true;
	std::vector<std::shared_ptr<Window>>& windowVec;  // ref of the windowVec, only for onEvent & pollingUpdate

	bool onEvent(Event & e);
};