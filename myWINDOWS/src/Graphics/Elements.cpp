#include "Elements.h"

#include <Windows.h>
#include <thread>

#include <AppHandler/AppHandler.h>

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

/* Label ****************************************************/

Label::Label(const wchar_t * cstr, Pos4 pos4, const Size& parentSize) :str(cstr), Elements(ElementsType::Button)
{
	// set values
	canvas = Canvas(pos4, { int(str.length()),1 }, parentSize);

	// update canvas
	Elements::flush();
}

/* Button ****************************************************/

Button::Button(const wchar_t * cstr, Pos4 pos4, const Size& parentSize, bool isFrame, Size padding) :str(cstr), Elements(ElementsType::Button)
{
	// set values
	canvas = Canvas(pos4, { int(str.length() + padding.width * 2),padding.height * 2 + 1 }, parentSize, isFrame, L'/');

	// update canvas
	Elements::flush();
}

void Button::flush_impl(wchar_t flushChar)
{
	flushChar? canvas.flush(flushChar): canvas.flush();

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

Inputbox::Inputbox(const wchar_t * cstr, Pos4 pos4, const Size& parentSize, int len, bool isFrame) :str(cstr), originStr(cstr), len(len), Elements(ElementsType::Inputbox)
{
	// set values
	canvas = Canvas(pos4, { len + 2,3 }, parentSize, isFrame, L' ');

	// update canvas
	flush();

	// pollingCallback
	setPollingCallback([this]() {
		static int i = 0;
		i = ++i % int(MY_UPDATE_FREQ);

		if (isFlash && (i == 0 || i == int(MY_UPDATE_FREQ / 2)))
		{
			isWhite = !isWhite, isWhite ? canvas.flush(L'¢i') : flush();
			return true;
		}

		return false;
	});
}

void Inputbox::flush_impl(wchar_t flushChar)
{
	flushChar ? canvas.flush(flushChar) : canvas.flush();


	std::wstring cstr = L"  " + str;
	canvas.line(1, 1, cstr.c_str(), cstr.length());
}

bool Inputbox::onMouseMove_impl(MouseMoveEvent & e)
{
	if (info.mouseHover == InfoType::Active && !isFlash)
		isFlash = true,	isWhite = false;
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

/* Image ****************************************************/

Image::Image(CharImage charImage,const Size& parentSize, bool isFrame) :charImage(charImage), Elements(ElementsType::Image)
{
	// set values
	canvas = Canvas(charImage.pos4, charImage.size, parentSize, isFrame);

	// update canvas
	Elements::flush();
}

/* Paragraph ****************************************************/

Paragraph::Paragraph(const wchar_t * cstr, Pos4 pos4, Size size, const Size & parentSize, 
	TextAlign textAlign, bool isFrame, wchar_t flushChar)
	: str(cstr), textAlign(textAlign), Elements(ElementsType::Paragraph)
{
	// set values
	canvas = Canvas(pos4, size, parentSize, isFrame, flushChar);

	// update canvas
	Elements::flush();
}

void Paragraph::flush_impl(wchar_t flushChar)
{
	flushChar ? canvas.flush(flushChar) : canvas.flush();

	int i = 1, start = 0, end, breakInd;
	do
	{
		if (str[start] == L'\n')
		{
			start++, i++;
			continue;
		}

		end = start + canvas.getSize().width - 2 > str.length() ? str.length() : start + canvas.getSize().width - 2;

		breakInd = str.find(L'\n', start + 1);
		if (breakInd + 1 && breakInd < end)
			end = breakInd;

		if (textAlign == TextAlign::left)
			canvas.line(1, i++, str.substr(start, end - start));
		else if (textAlign == TextAlign::center)
			canvas.lineCenter(canvas.getSize().width / 2, i++, str.substr(start, end - start));
		else
			canvas.line(canvas.getSize().width - (end - start) - 1, i++, str.substr(start, end - start));

		start = end;
		if (end == breakInd)
			i--;
	} while (end != str.length());
}
