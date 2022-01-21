#include "App.h"

bool App::onEvent(Event & e)
{
	if (e.getType() == EventType::shutdown)
		isRun = false;

	for (auto window = windowList.rbegin(); window != windowList.rend();)
	{
		bool ishandled = false;
		if ((*window)->onEvent(e))  // is handled
			ishandled = true;

		if (!(*window)->getIsRun())
		{
			window = std::list<std::shared_ptr<Window>>::reverse_iterator(windowList.erase(--window.base()));

			if (windowList.empty())  // no window exists
				isRun = false;
		}
		else if (ishandled)
			return true;
		else
			window++;
	}

	return false;
}

bool App::pollingUpdate()
{
	bool b = isNeedUpdate;
	for (auto&window : windowList)
		b |= (*window).pollingUpdate();

	isNeedUpdate = false;
	return b;
}