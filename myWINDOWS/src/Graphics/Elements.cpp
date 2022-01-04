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

bool Button::onMouseMove_impl(MouseMoveEvent & e)
{
	Pos& pos = canvas.getPos();
	Size& size = canvas.getSize();
	bool isNeedUpdate = false;

	if (e.getMouseX() >= pos.x&&e.getMouseX() < pos.x + size.width&&e.getMouseY() >= pos.y&&e.getMouseY() < pos.y + size.height)
	{
		if (!isHover)
			originStr = str, isHover = true, isNeedUpdate = true;

		if (mouseMoveCallback)
			mouseMoveCallback(*this);
	}
	else if (isHover)
		setString(originStr.c_str()), isHover = false, isNeedUpdate = true;

	return isNeedUpdate;
}

bool Button::onMousePrs_impl(MousePrsEvent & e)
{
	Pos& pos = canvas.getPos();
	Size& size = canvas.getSize();

	if (e.getMouseX() >= pos.x&&e.getMouseX() <= pos.x + size.width&&e.getMouseY() >= pos.y&&e.getMouseY() <= pos.y + size.height &&
		!isPrs)
	{
		flush(L'+'), isPrs = true;

		if (mousePrsCallback)
			mousePrsCallback(*this);
		return true;
	}
	return false;
}

bool Button::onMouseRls_impl(MouseRlsEvent & e)
{
	Pos& pos = canvas.getPos();
	Size& size = canvas.getSize();

	if (e.getMouseX() >= pos.x&&e.getMouseX() <= pos.x + size.width&&e.getMouseY() >= pos.y&&e.getMouseY() <= pos.y + size.height&&
		isPrs)
	{
		flush(), isPrs = false;

		if (mouseRlsCallback)
			mouseRlsCallback(*this);
		return true;
	}
	return false;
}