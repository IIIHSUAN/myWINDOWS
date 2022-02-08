#pragma once

#include <thread>
#include <mutex>
#include <queue>

#define MY_UPDATE_PERIOD 1000.0f/System::get().getFps()
#define MY_UPDATE_FREQ System::get().getFps()

#include "ImageT/ImageT.h"
#include "Graphics/Struct.h"
#include "Graphics/Canvas.h"

class Event;

class App;
enum class AppCollection;

class System
{
public:
	friend class Input;

	void run();  // entry point

	void createApp(AppCollection name);

	inline std::list<App*>& getAppList() { return appList; }
	inline const float& getFps() { return fps; }
	inline const time_t& getTime() { return time; }
	inline std::queue<Event*>& getEventQueue() { return eventQueue; }

	inline void setPollingPeriod(short millisecond) { pollingPeriod = millisecond; }
	inline void setIsPollingPeriod(bool b) { isPollingPeriod = b; }
	inline const bool& getIsPollingPeriod() { return isPollingPeriod; }

	inline void pushEvent(Event* e) { eventQueue.emplace(e); }

	void onResizeEvent(short width, short height);

	void shutdown();

	static System& get() { return *appHandler; }
private:
	static System* appHandler;
	std::thread pollingThread, inputThread, msgThread;
	std::list<std::thread> runThreadList;
	bool isRun = true, isForceRefresh = false;

	unsigned int msgCount = 0;
	std::wstring msgStr;
	Canvas msgCanvas = Canvas({ MY_WINDOW_WIDTH / 2 - 20, MY_WINDOW_HEIGHT / 2 - 4 }, { 40,8 }, false, L'\u25A2');
	bool isMsgRun = false;

	float fps = 10000.0f;
	time_t time;
	bool isPollingPeriod = false;
	short pollingPeriod = 20;  // millisecond
	void pollingUpdate();
	std::list<App*> appList;
	void update(std::list<App*>::iterator& app);
	Canvas canvas = Canvas({ 0,0 }, { MY_WINDOW_WIDTH,MY_WINDOW_HEIGHT }, false, L' ');
	Pos appStartPos = { 0,0 };

	std::queue<Event*> eventQueue;
	void onEvent(Event& e);
	void onKeyPrsEvent(unsigned short key, WCHAR unicodeChar);
	void System::onMouseMoveEvent(int X, int Y);
	void onMousePrsEvent(PollingInput input);
	void onMouseRlsEvent();
};