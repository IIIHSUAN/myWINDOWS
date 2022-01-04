#include "App.h"

bool App::onEvent(Event & e)
{
	for (auto window = windowVec.rbegin(); window != windowVec.rend();)
	{
		bool opcode = false;
		if ((*window)->onEvent(e))  // is handled
			opcode = true;

		if (!(*window)->getIsRun())
		{
			window = std::vector<std::shared_ptr<Window>>::reverse_iterator(windowVec.erase(--window.base()));

			if (windowVec.empty())  // no window exists
				isRun = false;
		}
		else if (opcode)
			return true;
		else
			window++;
	}

	return false;
}

bool App::pollingUpdate()
{
	bool b = isNeedUpdate;
	for (auto&window : windowVec)
		b |= (*window).pollingUpdate();

	isNeedUpdate = false;
	return b;
}