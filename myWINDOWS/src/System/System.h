#pragma once

#include <thread>

#define MY_UPDATE_PERIOD System::get().getPollingPeriod()*1000.0f
#define MY_UPDATE_FREQ 1.0f/System::get().getPollingPeriod()

#include "System/IO.h"
#include "ImageT/ImageT.h"
#include "App/App.h"
#include "App/AppCollection.h"
#include "Graphics/Struct.h"
#include "Graphics/Canvas.h"

class System
{
public:
	System();

	friend class Input;

	void run();  // entry point

	void createApp(AppCollection name);

	inline std::list<App*>& getAppList() { return appList; }
	inline const float& getPollingPeriod() { return pollingPeriod; }
	inline const float& getFps() { return fps; }
	inline const time_t& getTime() { return time; }

	inline void setPollingPeriod(float f) { pollingPeriod = f; }

	void shutdown();

	static System& get() { return *appHandler; }
private:
	static System* appHandler;
	std::thread pollingThread, inputThread, msgThread;
	std::list<std::thread> runThreadList;
	bool isRun = true;

	unsigned int isMsg = 0;
	std::wstring msgStr;
	Canvas msgCanvas = Canvas({ MY_WINDOW_WIDTH / 2 - 20, MY_WINDOW_HEIGHT / 2 - 4 }, { 40,8 }, false, L'\u25A2');
	bool isMsgRun = false;

	float fps = 0.0f;
	time_t time;
	float pollingPeriod = 0.027f;  // sec
	void pollingUpdate();
	std::list<App*> appList;
	void update(bool isFlush = true);
	Canvas canvas = Canvas({ 0,0 }, { MY_WINDOW_WIDTH,MY_WINDOW_HEIGHT }, false, L' ');

	std::function<void(Event&)> eventCallback;  // carry different events out
	void onEvent(Event& e);
	bool keyEvent(WORD key, DWORD ctrl, bool isPrs);
};