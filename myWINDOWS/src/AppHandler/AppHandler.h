#pragma once

#include <thread>

#define MY_UPDATE_PERIOD AppHandler::get().getPollingPeriod()*1000.0f
#define MY_UPDATE_FREQ 1.0f/AppHandler::get().getPollingPeriod()

#include "AppHandler/IO.h"
#include "Image/Image.h"
#include "App/App.h"
#include "App/AppCollection.h"
#include "Graphics/Struct.h"
#include "Graphics/Canvas.h"

class AppHandler
{
public:
	AppHandler();
	
	friend class Input;

	void run();  // entry point

	inline std::list<App*>& getAppList() { return appList; }
	void createApp(AppCollection name);

	unsigned int isMsg = 0;

	void shutdown();

	inline void setPollingPeriod(float f) { pollingPeriod = f; }
	inline const float& getPollingPeriod() { return pollingPeriod; }

	static AppHandler& get() { return *appHandler; }
private:
	static AppHandler* appHandler;
	std::thread pollingThread, inputThread, msgThread;
	std::list<std::thread> runThreadList;
	bool isRun = true;

	std::wstring msgStr;
	Canvas msgCanvas = Canvas({ MY_WINDOW_WIDTH / 2 - 20, MY_WINDOW_HEIGHT / 2 - 4 }, { 40,8 }, false, L'\u25A2');
	bool isMsgRun = false;

	float pollingPeriod = 0.025f;  //sec
	void pollingUpdate();
	std::list<App*> appList;
	void update(bool isFlush = true);
	Canvas canvas = Canvas({ 0,0 }, { MY_WINDOW_WIDTH,MY_WINDOW_HEIGHT }, false, L' ');

	std::function<void(Event&)> eventCallback;  // carry different events out
	void onEvent(Event& e);
	bool keyEvent(WORD key, DWORD ctrl, bool isPrs);
};