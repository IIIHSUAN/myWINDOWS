#include "Window.h"

Window::Window(App & _app, int _id, std::wstring & _name, Pos & _pos, Size & _size) :app(_app), id(_id), name(_name), pos(_pos), size(_size)
{
	size.width = min(size.width, MY_WINDOW_WIDTH), size.height = min(size.height, MY_WINDOW_HEIGHT);
	pos.y = max(min(pos.y, MY_WINDOW_HEIGHT - 2), 0);
	canvas = Canvas(pos, size, true);

	setTitle(name);
	canvas.getCanvas().replace(size.width - 5, 4, L"  x ");
}

bool Window::onEvent(Event & e)
{
	bool opcode = false;
	switch (e.getType())
	{
	case EventType::mouseMove:
		FUNC(opcode, windowMouseMove, app, *this, mouseMoveCallback, MouseMoveEvent, e);
		break;
	case EventType::mousePrs:
		FUNC(opcode, windowMouseClk, app, *this, mouseClkCallback, MousePrsEvent, e);
		break;
	case EventType::mouseRls:
		FUNC(opcode, windowMouseRls, app, *this, mouseRlsCallback, MouseRlsEvent, e);
		break;
	case EventType::keyPrs:
		FUNC(opcode, windowKeyPressed, app, *this, keyPressedCallback, KeyPrsEvent, e);
		break;
	}
	return opcode;
}

bool Window::windowRecieve(std::string str)
{
	return false;
}

bool Window::windowMouseMove(MouseMoveEvent & e)
{
	e.setPos({ e.getMouseX() - pos.x , e.getMouseY() - pos.y });
	if (e.getMouseX() < -2 || e.getMouseX() > size.width || e.getMouseY() < -2 || e.getMouseY() > size.height)
		return false;

	// move window
	if (e.getIsPressed() && 
		((e.getMouseY() == e.getOffsetY() && e.getMouseX() > 0 && e.getMouseX() < size.width) ||  // top frame
		(e.getMouseX() == e.getOffsetX()) && e.getMouseY() > 0 && e.getMouseY() < size.height))  // left frame
	{
		setWindowOffset(e.getOffsetX(), e.getOffsetY());
		return true;
	}

	// Elements
	bool isNeedUpdate = false;
	for (auto& ele : elementsVec)
		isNeedUpdate |= ele->onMouseMove(app, *this, e);
	if (isNeedUpdate)
		update();

	return false;
}

bool Window::windowMouseClk(MousePrsEvent & e)
{
	e.setPos({ e.getMouseX() - pos.x , e.getMouseY() - pos.y });
	if (e.getMouseX() < 0 || e.getMouseX() > size.width || e.getMouseY() < 0 || e.getMouseY() > size.height)
		return false;

	// click close
	static int rawPos; rawPos = index(e.getMouseY(), e.getMouseX());
	if (rawPos > size.width - 6 && rawPos < size.width - 2)
		isRun = false;

	// Elements
	bool isNeedUpdate = false;
	for (auto& ele : elementsVec)
		isNeedUpdate |= ele->onMouseClk(app, *this, e);
	if (isNeedUpdate)
		update();

	return false;
}

bool Window::windowMouseRls(MouseRlsEvent & e)
{
	e.setPos({ e.getMouseX() - pos.x , e.getMouseY() - pos.y });
	if (e.getMouseX() < 0 || e.getMouseX() > size.width || e.getMouseY() < 0 || e.getMouseY() > size.height)
		return false;

	// Elements
	bool isNeedUpdate = false;
	for (auto& ele : elementsVec)
		isNeedUpdate |= ele->onMouseRls(app, *this, e);
	if (isNeedUpdate)
		update();

	return false;
}

bool Window::windowKeyPressed(KeyPrsEvent & e)
{
	return false;
}

void Window::update()
{
	for(auto&ele:elementsVec)
		if(ele->getVisible())
			canvas.renderWithRel(ele->getCanvas());

	isNeedUpdate = true;
}