#include "App.h"

bool App::onEvent(Event & e)
{
	if (e.getType() == Event::shutdown)
		isRun = false;

	for (auto& window = windowList.rbegin(); window != windowList.rend(); window++)
		if ((*window)->onEvent(e))  // is handled
			return true;

	return false;
}

Status App::pollingUpdate()
{
	Status tmpStatus, status = Status::none;

	for (auto&window = windowList.begin(); window != windowList.end();)
	{
		tmpStatus = (*window)->pollingUpdate();
		
		if (tmpStatus > status)
			status = tmpStatus;

		if (!(*window)->getIsRun())
		{
			window = windowList.erase(window);

			if (windowList.empty())  // no window exists
				isRun = false;
		}
		else
			window++;
	}

	return status;
}