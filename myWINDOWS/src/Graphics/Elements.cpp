#include "Elements.h"

/* Button ****************************************************/

Button::Button(const wchar_t * cstr, Pos pos, bool isFrame) :Elements(ElementsType::Button)
{
	// set values
	str = cstr;
	Elements::getCanvas() = Canvas(pos, { int(str.length() + 16),5 }, isFrame, L'/');

	// update canvas
	Elements::flush();
}

void Button::flush_impl(wchar_t flushChar)
{
	if (flushChar)
		Elements::getCanvas().flush(flushChar);
	else
		Elements::getCanvas().flush();

	Elements::getCanvas().canvasCenterLine(str);
}

bool Button::onMouseMove_impl(App& app, Window& window, MouseMoveEvent & e)
{
	Pos& pos = canvas.getPos();
	Size& size = canvas.getSize();
	bool isNeedUpdate = false;

	if (e.getMouseX() >= pos.x&&e.getMouseX() < pos.x + size.width&&e.getMouseY() >= pos.y&&e.getMouseY() < pos.y + size.height)
	{
		if (!isHover)
			originStr = str, isHover = true, isNeedUpdate = true;

		if (mouseMoveCallback)
			mouseMoveCallback(app, window);
	}
	else if (isHover)
		setString(originStr.c_str()), isHover = false, isNeedUpdate = true;

	return isNeedUpdate;
}

bool Button::onMouseClk_impl(App& app, Window& window, MousePrsEvent & e)
{
	Pos& pos = canvas.getPos();
	Size& size = canvas.getSize();

	if (e.getMouseX() >= pos.x&&e.getMouseX() <= pos.x + size.width&&e.getMouseY() >= pos.y&&e.getMouseY() <= pos.y + size.height &&
		!isClk)
	{
		flush(L'+'), isClk = true;

		if (mouseClkCallback)
			mouseClkCallback(app, window);
		return true;
	}
	return false;
}

bool Button::onMouseRls_impl(App& app, Window& window, MouseRlsEvent & e)
{
	Pos& pos = canvas.getPos();
	Size& size = canvas.getSize();

	if (e.getMouseX() >= pos.x&&e.getMouseX() <= pos.x + size.width&&e.getMouseY() >= pos.y&&e.getMouseY() <= pos.y + size.height&&
		isClk)
	{
		flush(), isClk = false;

		if (mouseRlsCallback)
			mouseRlsCallback(app, window);
		return true;
	}
	return false;
}