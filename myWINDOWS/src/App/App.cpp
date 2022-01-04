#include "App.h"

bool App::onEvent(Event & e)
{
	for (auto window = windowVec.rbegin(); window != windowVec.rend();)
	{
		if ((*window)->onEvent(e))  // is handled
			return true;

		if (!(*window)->getIsRun())
		{
			auto it = windowVec.erase(--window.base());
			window = std::vector<std::shared_ptr<Window>>::reverse_iterator(it);	

			if (windowVec.empty())  // no window exists
				isRun = false;
		}
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

void App::createWindow(std::wstring name, Pos pos, Size size)
{
	if (pos == 0)
		pos = subWindowPos, subWindowPos.x += 2, subWindowPos.y += 1;
	windowVec.emplace_back(std::make_shared<Window>(*this, windowIdNum++, name, pos, size));
}