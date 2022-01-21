#include "Window.h"

Window::Window(int _id, std::wstring & _name, Pos & _pos, Size & _size, const wchar_t flushChar) : id(_id), name(_name), pos(_pos), size(_size)
{
	size.width = min(size.width, MY_WINDOW_WIDTH), size.height = min(size.height, MY_WINDOW_HEIGHT);
	pos.y = max(min(pos.y, MY_WINDOW_HEIGHT - 2), 0);
	canvas = Canvas(pos, size, true, flushChar);
}

bool Window::onEvent(Event & e)
{
	bool isHandled = false;  // isHandled
	switch (e.getType())
	{
	case EventType::mouseMove:
		FUNC(isHandled, windowMouseMove, app, *this, mouseMoveCallback, MouseMoveEvent, e);
		break;
	case EventType::mousePrs:
		FUNC(isHandled, windowMousePrs, app, *this, mousePrsCallback, MousePrsEvent, e);
		break;
	case EventType::mouseRls:
		FUNC(isHandled, windowMouseRls, app, *this, mouseRlsCallback, MouseRlsEvent, e);
		break;
	case EventType::keyPrs:
		FUNC(isHandled, windowKeyPrs, app, *this, keyPrsCallback, KeyPrsEvent, e);
		break;
	case EventType::shutdown:
		isRun = false;
		break;
	}
	return isHandled;
}

bool Window::windowRecieve(std::string str)
{
	return false;
}

bool Window::windowMouseMove(MouseMoveEvent e)
{
	e.setPos({ e.getMouseX() - pos.x , e.getMouseY() - pos.y });
	if (e.getMouseX() < -1 || e.getMouseX() > size.width || e.getMouseY() < -1 || e.getMouseY() > size.height)
		return false;

	// resize window
	int direction = 0;
	if (e.getIsPrs() && (
		(e.getMouseX() <= 1			 && e.getMouseY() <= 1					 && (direction = 1)) ||  // top-left
		(e.getMouseX() >= size.width - 2 && e.getMouseY() <= 1				 && (direction = 2)) ||  // top-right
		(e.getMouseX() >= size.width - 2 && e.getMouseY() >= size.height - 2 && (direction = 3)) ||  // bottom-right
		(e.getMouseX() <= 1			 && e.getMouseY() >= size.height - 2 && (direction = 4))     // bottom-left
		) && direction)
	{
		if (size.width >= 20 && size.width <= MY_WINDOW_WIDTH)
			if (direction == 1 || direction == 4)
				pos.x += e.getOffsetX(), size.width -= e.getOffsetX();
			else
				size.width += e.getOffsetX();

		if (size.height >= 5 && size.height <= MY_WINDOW_HEIGHT)
			if (direction <= 2)
				pos.y += e.getOffsetY(), size.height -= e.getOffsetY();
			else
				size.height += e.getOffsetY();

		canvas.setSize(size);
		for (auto& ele : elementsList)
			e.getOffsetX() ? ele->getCanvas().setParentWidth(size.width) : ele->getCanvas().setParentHeight(size.height);

		refresh();
		return true;
	}

	// move window
	else if (e.getIsPrs() &&
		((e.getMouseY() == e.getOffsetY() && e.getMouseX() > 0 && e.getMouseX() < size.width) ||  // top frame
		(e.getMouseX() == e.getOffsetX()) && e.getMouseY() > 0 && e.getMouseY() < size.height))  // left frame
	{
		setWindowOffset(e.getOffsetX(), e.getOffsetY());
		return true;
	}

	// Elements
	bool isHandled = false;
	std::list<std::shared_ptr<Elements>>::iterator frontRenderEle = elementsList.end();
	for (auto ele = elementsList.rbegin(); ele != elementsList.rend(); ele++)
	{
		MouseMoveEvent ee = e;
		isHandled = (*ele)->onMouseMove(ee);
		if ((*ele)->pendingUpdate() && (*ele)->getVisible())
		{
			isNeedUpdate = true;
			frontRenderEle = std::next(ele.base(), -1);
		}

		if (isHandled)
			break;
	}

	// render from front Elements
	for (auto ele2 = frontRenderEle; ele2 != elementsList.end(); ele2++)
		canvas.renderWith((*ele2)->getCanvas());

	return true;
}

bool Window::windowMousePrs(MousePrsEvent e)
{
	e.setPos({ e.getMouseX() - pos.x , e.getMouseY() - pos.y });
	if (e.getMouseX() < 0 || e.getMouseX() > size.width || e.getMouseY() < 0 || e.getMouseY() > size.height)
		return false;

	// click close
	if ((e.getMouseY() == 0) && (e.getMouseX() > size.width - 6 && e.getMouseX() < size.width - 2))
	{
		isRun = false, isNeedUpdate = true;
		return true;
	}
	// Elements
	bool isHandled = false;
	std::list<std::shared_ptr<Elements>>::iterator frontRenderEle = elementsList.end();
	for (auto ele = elementsList.rbegin(); ele != elementsList.rend(); ele++)
	{
		MousePrsEvent ee = e;
		isHandled = (*ele)->onMousePrs(ee);
		if ((*ele)->pendingUpdate() && (*ele)->getVisible())
		{
			isNeedUpdate = true;
			frontRenderEle = std::next(ele.base(), -1);
		}

		if (isHandled)
			break;
	}

	// render from front Elements
	for (auto ele2 = frontRenderEle; ele2 != elementsList.end(); ele2++)
		canvas.renderWith((*ele2)->getCanvas());

	return true;
}

bool Window::windowMouseRls(MouseRlsEvent e)
{
	e.setPos({ e.getMouseX() - pos.x , e.getMouseY() - pos.y });
	if (e.getMouseX() < 0 || e.getMouseX() > size.width || e.getMouseY() < 0 || e.getMouseY() > size.height)
		return false;

	// Elements
	bool isHandled = false;
	std::list<std::shared_ptr<Elements>>::iterator frontRenderEle = elementsList.end();
	for (auto ele = elementsList.rbegin(); ele != elementsList.rend(); ele++)
	{
		MouseRlsEvent ee = e;
		isHandled = (*ele)->onMouseRls(ee);
		if ((*ele)->pendingUpdate() && (*ele)->getVisible())
		{
			isNeedUpdate = true;
			frontRenderEle = std::next(ele.base(), -1);
		}

		if (isHandled)
			break;
	}

	// render from front Elements
	for (auto ele2 = frontRenderEle; ele2 != elementsList.end(); ele2++)
		canvas.renderWith((*ele2)->getCanvas());

	return true;
}

bool Window::windowKeyPrs(KeyPrsEvent e)
{
	// Elements
	bool isWindowHandled = false;
	std::list<std::shared_ptr<Elements>>::iterator frontRenderEle = elementsList.end();
	for (auto ele = elementsList.rbegin(); ele != elementsList.rend(); ele++)
	{
		KeyPrsEvent ee = e;
		isWindowHandled |= (*ele)->onKeyPrs(ee);
		if ((*ele)->pendingUpdate() && (*ele)->getVisible())
		{
			isNeedUpdate = true;
			frontRenderEle = std::next(ele.base(), -1);
		}
	}

	// render from front Elements
	for (auto ele2 = frontRenderEle; ele2 != elementsList.end(); ele2++)
		canvas.renderWith((*ele2)->getCanvas());

	return isWindowHandled;
}

void Window::zindex(unsigned int& ind, unsigned int& ele_zindex)
{
	// ind to ele_zindex ind
	ele_zindex = max(min(ele_zindex, int(elementsList.size() - 1)), 0);

	auto it = std::next(elementsList.begin(), ind);
	auto rb = std::next(it);
	auto re = std::next(elementsList.begin(), ele_zindex + (ind < ele_zindex));
	elementsList.splice(it, elementsList, rb, re);
}

void Window::ajustZindex()
{
	unsigned int ind = 0; bool isAjustBack = false;
	for (auto ele = elementsList.begin(); ele != elementsList.end();)
	{
		if (isAjustBack && !(*ele)->isForceZindex)
			(*ele)->setZindex(ind);
		else if ((*ele)->pollingZindex() && ind != (*ele)->zindex)
		{
			if ((*ele)->zindex > ind)
				zindex(ind, (*ele)->zindex), ele = std::next(elementsList.begin(), ind);
			else
				zindex(ind, (*ele)->zindex);

			isAjustBack = true;
			continue;
		}

		ele++, ind++;
	}
}

void Window::refresh()
{
	canvas.flush();

	elementsUpdate(true);

	canvas.frame();
	setTitle(name),	canvas.getCanvas().replace(size.width - 5, 4, L"  x ");
}

bool Window::pollingUpdate()
{
	if (isForcePollingRefresh)
		refresh();
	else
		elementsUpdate();
	
	if (pollingCallback)
		pollingCallback();

	bool b = isNeedUpdate; isNeedUpdate = false;
	return b;
}

void Window::elementsUpdate(bool forceUpdate)
{
	isForcePollingRefresh = false;

	// ajust zindex first
	ajustZindex();
	
	// pollingUpdate
	std::list<std::shared_ptr<Elements>>::iterator frontRenderEle = elementsList.end();
	for (auto ele = elementsList.rbegin(); ele != elementsList.rend(); ele++)
		if (((*ele)->onPollingUpdate(isForcePollingRefresh) && (*ele)->getVisible()) || forceUpdate)
		{
			isNeedUpdate = true;
			frontRenderEle = std::next(ele.base(), -1);
		}

	// render from front Elements
	for (auto ele2 = frontRenderEle; ele2 != elementsList.end(); ele2++)
		canvas.renderWith((*ele2)->getCanvas());
}