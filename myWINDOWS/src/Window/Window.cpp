#include "Window.h"

Window::Window(int _id, std::wstring & _name, Pos & _pos, Size & _size) : id(_id), name(_name), pos(_pos), size(_size)
{
	size.width = min(size.width, MY_WINDOW_WIDTH), size.height = min(size.height, MY_WINDOW_HEIGHT);
	pos.y = max(min(pos.y, MY_WINDOW_HEIGHT - 2), 0);
	canvas = Canvas(pos, size, true);

	setTitle(name);
	canvas.getCanvas().replace(size.width - 5, 4, L"  x ");
}

bool Window::onEvent(Event & e)
{
	bool opcode = false;  // isHandled
	switch (e.getType())
	{
	case EventType::mouseMove:
		FUNC(opcode, windowMouseMove, app, *this, mouseMoveCallback, MouseMoveEvent, e);
		break;
	case EventType::mousePrs:
		FUNC(opcode, windowMousePrs, app, *this, mousePrsCallback, MousePrsEvent, e);
		break;
	case EventType::mouseRls:
		FUNC(opcode, windowMouseRls, app, *this, mouseRlsCallback, MouseRlsEvent, e);
		break;
	case EventType::keyPrs:
		FUNC(opcode, windowKeyPrs, app, *this, keyPrsCallback, KeyPrsEvent, e);
		break;
	case EventType::shutdown:
		isRun = false;
		break;
	}
	return opcode;
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

	// move window
	if (e.getIsPrs() &&
		((e.getMouseY() == e.getOffsetY() && e.getMouseX() > 0 && e.getMouseX() < size.width) ||  // top frame
		(e.getMouseX() == e.getOffsetX()) && e.getMouseY() > 0 && e.getMouseY() < size.height))  // left frame
	{
		setWindowOffset(e.getOffsetX(), e.getOffsetY());
		return true;
	}

	// Elements
	bool opcode = false;
	for (auto& ele : elementsVec)
	{
		MouseMoveEvent ee = e;
		opcode |= ele->onMouseMove(ee);
		if (ele->pendingUpdate() && ele->getVisible())
			canvas.renderWithRel(ele->getCanvas()), isNeedUpdate = true;
	}

	return true;// opcode;
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
	bool opcode = false;
	for (auto& ele : elementsVec)
	{
		MousePrsEvent ee = e;
		opcode |= ele->onMousePrs(ee);
		if (ele->pendingUpdate() && ele->getVisible())
			canvas.renderWithRel(ele->getCanvas()), isNeedUpdate = true;
	}

	return true;// opcode;
}

bool Window::windowMouseRls(MouseRlsEvent e)
{
	e.setPos({ e.getMouseX() - pos.x , e.getMouseY() - pos.y });
	if (e.getMouseX() < 0 || e.getMouseX() > size.width || e.getMouseY() < 0 || e.getMouseY() > size.height)
		return false;

	// Elements
	bool opcode = false;
	for (auto& ele : elementsVec)
	{
		MouseRlsEvent ee = e;
		opcode |= ele->onMouseRls(ee);
		if (ele->pendingUpdate() && ele->getVisible())
			canvas.renderWithRel(ele->getCanvas()), isNeedUpdate = true;
	}

	return true;// opcode;
}

bool Window::windowKeyPrs(KeyPrsEvent e)
{
	// Elements
	bool opcode = false;
	for (auto& ele : elementsVec)
	{
		KeyPrsEvent ee = e;
		opcode |= ele->onKeyPrs(ee);
		if (ele->pendingUpdate() && ele->getVisible())
			canvas.renderWithRel(ele->getCanvas()), isNeedUpdate = true;
	}

	return opcode;
}

bool Window::pollingUpdate()
{
	elementsUpdate();
	bool b = isNeedUpdate; isNeedUpdate = false; return b;
}
void Window::elementsUpdate()
{
	bool opcode = false;
	for (auto&ele : elementsVec)
	{
		if (ele->pendingUpdate() && ele->getVisible())
			canvas.renderWithRel(ele->getCanvas()), opcode |= true;		
	}
	
	isNeedUpdate |= opcode;
}