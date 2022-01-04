#pragma once

#include <list>

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

	inline std::vector<App*>& getAppVec() { return appVec; }
	void createApp(AppCollection name);

	unsigned int isMsg = 0;

	void shutdown();

	static AppHandler& get() { return *appHandler; }
private:
	static AppHandler* appHandler;
	bool isRun = true;
	Mouse& mouse = Mouse::get();

	std::wstring msgStr;
	Canvas msgCanvas = Canvas({ MY_WINDOW_WIDTH / 2 - 20, MY_WINDOW_HEIGHT / 2 - 4 }, { 40,8 }, false, L'\u25A2');
	bool isMsgRun = false;

	float pollingPeriod = 0.4f;  //sec
	void pollingUpdate();
	std::vector<App*> appVec;
	void update(bool isFlush = true);
	Canvas canvas = Canvas({ 0,0 }, { MY_WINDOW_WIDTH,MY_WINDOW_HEIGHT }, false, L'.');

	std::function<void(Event&)> eventCallback;  // carry different events out
	void onEvent(Event& e);
	bool keyEvent(WORD key, DWORD ctrl, bool isPrs);
};