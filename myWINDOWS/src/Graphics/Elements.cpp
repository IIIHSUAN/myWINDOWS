#include "Elements.h"

#include <Windows.h>
#include <thread>

void Elements::flush_impl(wchar_t flushChar)
{
	if (flushChar)
		canvas.flush(flushChar);
	else
		canvas.flush();
}

bool Elements::_onMouseMove(MouseMoveEvent & e)
{
	Pos& pos = canvas.getPos();
	Size& size = canvas.getSize();
	e.setPos({ e.getMouseX() - pos.x , e.getMouseY() - pos.y });

	bool isCallback = false;
	if (e.getMouseX() >= 0 && e.getMouseX() < size.width&&e.getMouseY() >= 0 && e.getMouseY() < size.height)
		info.mouseHover = InfoType::Active, isCallback = true;
	else if (info.mouseHover == InfoType::Active)
		info.mouseHover = InfoType::Cancel, isCallback = true;
	else if (info.mouseHover == InfoType::Cancel)
		info.mouseHover = InfoType::None;

	if (info.mouseClick == InfoType::Active)
		info.mouseClick = InfoType::Cancel, isCallback = true;
	else if (info.mouseClick == InfoType::Cancel)
		info.mouseClick = InfoType::None;

	return isCallback;
}

bool Elements::_onMousePrs(MousePrsEvent & e)
{
	Pos& pos = canvas.getPos();
	Size& size = canvas.getSize();
	e.setPos({ e.getMouseX() - pos.x , e.getMouseY() - pos.y });

	bool isCallback = false;
	if (e.getMouseX() >= 0 && e.getMouseX() < size.width&&e.getMouseY() >= 0 && e.getMouseY() < size.height)
		info.mousePressed = InfoType::Active, isCallback = true;

	return isCallback;
}

bool Elements::_onMouseRls(MouseRlsEvent & e)
{
	Pos& pos = canvas.getPos();
	Size& size = canvas.getSize();
	e.setPos({ e.getMouseX() - pos.x , e.getMouseY() - pos.y });

	bool isCallback = false;
	if (e.getMouseX() >= 0 && e.getMouseX() < size.width&&e.getMouseY() >= 0 && e.getMouseY() < size.height
		&& info.mousePressed == InfoType::Active)
		info.mouseClick = InfoType::Active, isCallback = true;

	if (info.mousePressed == InfoType::Active)
		info.mousePressed = InfoType::Cancel, isCallback = true;
	else if (info.mousePressed == InfoType::Cancel)
		info.mousePressed = InfoType::None;

	return isCallback;
}

/* Button ****************************************************/

Button::Button(const wchar_t * cstr, Pos pos, bool isFrame) :str(cstr), Elements(ElementsType::Button)
{
	// set values
	canvas = Canvas(pos, { int(str.length() + 16),5 }, isFrame, L'/');

	// update canvas
	Elements::flush();
}

void Button::flush_impl(wchar_t flushChar)
{
	if (flushChar)
		canvas.flush(flushChar);
	else
		canvas.flush();

	canvas.canvasCenterLine(str);
}

bool Button::onMouseMove_impl(MouseMoveEvent & e)
{
	if (info.mouseHover == InfoType::Active)
		Elements::flush(L'+');
	else
		Elements::flush();

	bool opcode = false;
	if (mouseHoverCallback)
		opcode |= mouseHoverCallback(*this);
	return opcode;
}

bool Button::onMousePrs_impl(MousePrsEvent & e)
{
	if (info.mousePressed == InfoType::Active)
		Elements::flush(L'|');

	return false;
}

bool Button::onMouseRls_impl(MouseRlsEvent & e)
{
	bool opcode = false;
	if (info.mousePressed == InfoType::Cancel)
		Elements::flush();

	if (info.mouseClick == InfoType::Active && mouseClkCallback)
		opcode |= mouseClkCallback(*this);
	return opcode;
}

/* Inputbox ****************************************************/

Inputbox::Inputbox(const wchar_t * cstr, Pos pos, int len, bool isFrame) :str(cstr), originStr(cstr), len(len), Elements(ElementsType::Inputbox)
{
	// set values
	canvas = Canvas(pos, { len + 2,3 }, isFrame, L' ');

	// update canvas
	Elements::flush();
}

void Inputbox::flush_impl(wchar_t flushChar)
{
	if (flushChar)
		canvas.flush(flushChar);
	else
		canvas.flush(L' ');

	std::wstring cstr = L"  " + str;
	canvas.line(1, 1, cstr.c_str(), cstr.length());
}

bool Inputbox::onMouseMove_impl(MouseMoveEvent & e)
{
	if (info.mouseHover == InfoType::Active && !isFlash)
	{
		isFlash = true;

		std::thread t([this]() {
			bool isOn = false;
			while (info.mouseHover == InfoType::Active && isFlash)
				isOn = !isOn, isOn ? canvas.flush(L'¢i') : Elements::flush(), isNeedUpdate = true, Sleep(500);

 			Elements::flush();
			isFlash = false;
		});
		t.detach();
	}
	else if (info.mouseHover == InfoType::Cancel)
	{
		if(isFlash)
			isFlash = false;
		if (isFocus)
			isFocus = false;

		str = originStr, flush();
	}
	
	return false;
}

bool Inputbox::onMouseRls_impl(MouseRlsEvent & e)
{
	if (info.mouseClick == InfoType::Active)
		originStr = str, setString(L""), isFocus = true;

	return false;
}

bool Inputbox::onKeyPrs_impl(KeyPrsEvent & e)
{
	bool opcode = false;
	if (isFocus)
	{
		isFlash = false;

		if(e.getKey() == VK_RETURN)
			originStr = str;
		else if (e.getKey() == VK_BACK)
			str = str.substr(0, str.length() - 1);
		else
			str += e.getKey();
		flush();

		if(keyPrsCallback)
			opcode |= keyPrsCallback(e, str);
	}
	
	return opcode;
}

/* Panel ****************************************************/

Panel::Panel(Pos pos, Size size, bool isFrame, CharImage * charImage) :Elements(ElementsType::Panel), charImage(charImage)
{
	// set values
	canvas = Canvas(pos, size, isFrame, L'+');

	// update canvas
	Elements::flush();
}

void Panel::flush_impl(wchar_t flushChar)
{
	canvas.flush();

	if (charImage)
		canvas.setBackground(*charImage);
}