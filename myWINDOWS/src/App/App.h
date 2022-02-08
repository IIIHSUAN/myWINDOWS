#pragma once

#include <list>
#include "Window/Window.h"
#include "Window/Collection/DevToolWindow.h"

#define push_window(window_ptr, window_entity) window_ptr.reset(new window_entity), pushWindow(window_ptr)

class System;

enum class AppCollection { unknown, Desktop, Settings, Painter, Chess, CubeViewer };
class App
{
public:
	App(AppCollection app, UINT id) : app(app), id(id) {}

	friend System;

	inline void setWindowList(std::list<std::shared_ptr<Window>>& _windowList) { windowList = _windowList; }
	inline std::list<std::shared_ptr<Window>>& getWindowList() { return windowList; }

	inline AppCollection getApp() { return app; }
	inline UINT getId() { return id; }
	inline bool getIsRun() { return isRun; }

	Status pollingUpdate();

	virtual void run() {}

	inline void pushWindow(std::shared_ptr<Window>& window) {
		windowList.emplace_back(window), window->setId(unsigned int(windowList.size() - 1)), window->refresh();
	}
protected:
	void closeApp() { isRun = false; }
private:
	AppCollection app;
	UINT id;
	std::list<std::shared_ptr<Window>> windowList;

	bool isRun = true;
	bool onEvent(Event & e);
};