#include "Window.h"
#include "System/IO.h"

Window::Window(int _id, std::wstring & _name, Pos & _pos, Size & _size, const wchar_t flushChar, WindowCollection window)
	: id(_id), name(_name), pos(_pos), size(_size), window(window)
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
	case Event::mouseMove:
		isHandled |= _onMouseMove((MouseMoveEvent&)e);
		break;
	case Event::mousePrs:
		isHandled |= _onMousePrs((MousePrsEvent&)e);
		break;
	case Event::mouseRls:
		isHandled |= _onMouseRls((MouseRlsEvent&)e);
		break;
	case Event::keyPrs:
		isHandled |= _onKeyPrs((KeyPrsEvent&)e);
		break;
	case Event::recv:
		break;
	case Event::shutdown:
		isRun = false;
		break;
	}
	return isHandled;
}

bool Window::elementsOnEvent(Event & e)
{
	// Elements
	bool isHandled = false, isRefresh = false;
	std::list<std::shared_ptr<Elements>>::iterator frontRenderEle = elementsList.end();

	for (auto ele = elementsList.rbegin(); ele != elementsList.rend(); ele++)
	{
		switch (e.getType())  // pass Event "by value"
		{
		case Event::mouseMove:
			isHandled = (*ele)->onMouseMove((MouseMoveEvent&)e);
			mouseInfo.handledElementInd = isHandled ? unsigned int(std::distance(elementsList.begin(), ele.base())) : 0;
			mouseInfo.setLast({ Mouse::get().X,Mouse::get().Y });
			break;
		case Event::mousePrs:
 			isHandled = (*ele)->onMousePrs((MousePrsEvent&)e);
			break;
		case Event::mouseRls:
			isHandled = (*ele)->onMouseRls((MouseRlsEvent&)e);
			break;
		case Event::keyPrs:
			isHandled = (*ele)->onKeyPrs((KeyPrsEvent&)e);
			break;
		case Event::recv:
			break;
		case Event::windowResize:
			(*ele)->onWindowResize((WindowResizeEvent&)e);
			isRefresh = true;
			break;
		case Event::shutdown:
			break;
		}
		
		if ((*ele)->pendingUpdate() && (*ele)->getVisible())
		{
			isNeedUpdate = true;
			frontRenderEle = std::next(ele.base(), -1);
		}

		if (isHandled)
			break;
	}

	if (isRefresh)
		refresh();
	else
		// render from front Elements
		for (auto ele2 = frontRenderEle; ele2 != elementsList.end(); ele2++)
			canvas.renderWith((*ele2)->getCanvas());

	return isHandled;
}

void Window::_onRecv(std::string str)
{
}
bool Window::_onMouseMove(MouseMoveEvent e)
{
	e.setPos({ e.getMouseX() - pos.x , e.getMouseY() - pos.y });
	if (!(e.getMouseX() >= 0 && e.getMouseX() < size.width && e.getMouseY() >= 0 && e.getMouseY() < size.height))
		return false;

	if (!e.getIsPrs())
	{
		if (mouseInfo.isPrsCorner) mouseInfo.isPrsCorner = false;
		if (mouseInfo.isPrsFrame) mouseInfo.isPrsFrame = false;
	}

	// resize window
	int direction = 0;
	if (mouseInfo.isPrsCorner && (
		(e.getMouseX() <= 1				 && e.getMouseY() <= 1				 && (direction = 1)) ||  // top-left
		(e.getMouseX() >= size.width - 2 && e.getMouseY() <= 1				 && (direction = 2)) ||  // top-right
		(e.getMouseX() >= size.width - 2 && e.getMouseY() >= size.height - 2 && (direction = 3)) ||  // bottom-right
		(e.getMouseX() <= 1				 && e.getMouseY() >= size.height - 2 && (direction = 4))     // bottom-left
		) && direction)
	{
		if (!(size.width < 25 && e.getMouseX() > 0 && e.getMouseX() < size.width) &&
			!(size.width >= MY_WINDOW_WIDTH && !(e.getMouseX() > 0 && e.getMouseX() < size.width)))
		{
			if (direction == 1 || direction == 4)  // left
				pos.x += e.getOffsetX(), size.width -= e.getOffsetX();
			else  // right
				size.width += e.getOffsetX();
		}
		if (!(size.height < 5 && e.getMouseY() > 0 && e.getMouseY() < size.height) &&
			!(size.height >= MY_WINDOW_HEIGHT && !(e.getMouseY() > 0 && e.getMouseY() < size.height)))
		{
			if (direction <= 2)  // top
				pos.y += e.getOffsetY(), size.height -= e.getOffsetY();
			else  // bottom
				size.height += e.getOffsetY();
		}
		

		WindowResizeEvent re(canvas.getPos(), canvas.getSize(), pos, size);

		canvas.setPos(pos), canvas.setSize(size);
		elementsOnEvent(re);

		if (resizeCallback) resizeCallback(re);
		return true;
	}
	// move window
	else if (mouseInfo.isPrsFrame)
	{
		pos.x += e.getOffsetX(), pos.y += e.getOffsetY();
		
		WindowResizeEvent re(canvas.getPos(), canvas.getSize(), pos, size);

		canvas.setPos(pos);
		elementsOnEvent(re);

		if (resizeCallback) resizeCallback(re);
		return true;
	}

	elementsOnEvent(e);

	if (mouseMoveCallback) mouseMoveCallback(e);
	return true;
}
bool Window::_onMousePrs(MousePrsEvent e)
{
	e.setPos({ e.getMouseX() - pos.x , e.getMouseY() - pos.y });
	if (e.getMouseX() < 0 || e.getMouseX() > size.width || e.getMouseY() < 0 || e.getMouseY() > size.height)
		return false;

	// click close
	if ((e.getMouseY() == 0) && (e.getMouseX() > size.width - 6 && e.getMouseX() < size.width - 2)) {
		isRun = false, isNeedUpdate = true;	return true;
	}
	// click corner
	if ((e.getMouseX() == 0 || e.getMouseX() == size.width - 1) && (e.getMouseY() == 0 || e.getMouseY() == size.height - 1) && (mouseInfo.isPrsCorner = true))
		return true;
	// click frame
	else if ((e.getMouseX() == 0 || e.getMouseX() == size.width - 1 || e.getMouseY() == 0 || e.getMouseY() == size.height - 1) && (mouseInfo.isPrsFrame = true))
		return true;

	elementsOnEvent(e);

	if (mousePrsCallback) mousePrsCallback(e);
	return true;
}
bool Window::_onMouseRls(MouseRlsEvent e)
{
	e.setPos({ e.getMouseX() - pos.x , e.getMouseY() - pos.y });
	if (e.getMouseX() < 0 || e.getMouseX() > size.width || e.getMouseY() < 0 || e.getMouseY() > size.height)
		return false;


	elementsOnEvent(e);

	if (mouseRlsCallback) mouseRlsCallback(e);
	return true;
}
bool Window::_onKeyPrs(KeyPrsEvent e)
{
	bool isHandled = false;
	isHandled = elementsOnEvent(e);

	if (keyPrsCallback) isHandled |= keyPrsCallback(e);
	return isHandled;
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

void Window::adjustZindex()
{
	unsigned int ind = 0; bool isAjustBack = false;
	for (auto ele = elementsList.begin(); ele != elementsList.end();)
	{
		if (isAjustBack && !(*ele)->isForceZindex)
			(*ele)->setZindex(ind);
		else if ((*ele)->pollingZindex() && ind != (*ele)->zindex)
		{
			if (ind < (*ele)->zindex)  // origin less than target
			{
				zindex(ind, (*ele)->zindex), ele = std::next(elementsList.begin(), ind - (ind != 0));
				continue;
			}
			else
				zindex(ind, (*ele)->zindex), ind = (*ele)->zindex;

			isAjustBack = true;
			
		}

		ele++, ind++;
	}
}

void Window::refresh()
{
	canvas.flush();

	// render Elements
	for (auto ele = elementsList.begin(); ele != elementsList.end(); ele++)
		canvas.renderWith((*ele)->getCanvas());

	canvas.frame();
	setTitle(name), canvas.line(size.width - 5, 0, L"  x ");
}

bool Window::pollingUpdate()
{
	elementsUpdate();

	if (pollingCallback) pollingCallback();

	bool b = isNeedUpdate; isNeedUpdate = false; return b;
}

void Window::elementsUpdate()
{
	bool isRefresh = false;
	PollingStatus pollingStatus;

	// adjust zindex first
	adjustZindex();
	
	// pollingUpdate
	std::list<std::shared_ptr<Elements>>::iterator frontRenderEle = elementsList.end();
	for (auto ele = elementsList.rbegin(); ele != elementsList.rend(); ele++)
	{
		pollingStatus = (*ele)->onPollingUpdate(mouseInfo);
		if (pollingStatus == PollingStatus::needUpdate)
		{
			if ((*ele)->getVisible())
				isNeedUpdate |= true, frontRenderEle = std::next(ele.base(), -1);
		}
		else if (pollingStatus == PollingStatus::refresh)
			isRefresh |= true;
	}

	if (isRefresh)
		refresh();
	else
	{
		// render from front Elements
		for (auto ele2 = frontRenderEle; ele2 != elementsList.end(); ele2++)
			canvas.renderWith((*ele2)->getCanvas());
	}
}