#pragma once

#include <vector>

#include "Window/Window.h"
#include "AppCollection.h"

class AppHandler;

#define PUSH_WINDOW(window_ptr, window_entity) window_ptr.reset(new window_entity), pushWindow(window_ptr)

/*
 * template
 * public get App::member via public:
 * child using App::member via App::func()
 */

class App
{
public:
	App(AppCollection app) : app(app) {}

	friend AppHandler;

	inline void setWindowVec(std::vector<std::shared_ptr<Window>>& _windowVec) { windowVec = _windowVec; }
	inline std::vector<std::shared_ptr<Window>>& getWindowVec() { return windowVec; }

	inline AppCollection& getApp() { return app; }
	inline bool& getIsRun() { return isRun; }

	bool pollingUpdate();

	virtual void run() {}

	inline void pushWindow(std::shared_ptr<Window>& window) { windowVec.emplace_back(window), window->refresh(); }
protected:
	bool isRun = true, isNeedUpdate = true;
private:
	AppCollection app;
	std::vector<std::shared_ptr<Window>> windowVec;

	bool onEvent(Event & e);
};