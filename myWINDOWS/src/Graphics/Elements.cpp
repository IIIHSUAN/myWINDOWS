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

bool Elements::onPollingUpdate(bool& isForceWindowRefresh)
{
	bool b = isNeedUpdate;
	if (pollingCallback) b |= pollingCallback();
	if(animFunc) animFunc(isAnimInit), isForceWindowRefresh |= true;
	if (animCallback) animCallback(), animCallback = nullptr;
	isNeedUpdate = false; return b;
}

void Elements::animate(Animate animAttr, std::function<void()> completeCallback)
{
	float dx, dy, dw, dh, dt = AppHandler::get().getPollingPeriod(),
		xx = float(canvas.getPos().x), yy = float(canvas.getPos().y), ww = float(canvas.getSize().width), hh = float(canvas.getSize().height), tt = 0.0f;

	if (animAttr.isPosAnim)
		dx = (animAttr.toPos.x - xx)* dt / animAttr.duration,
		dy = (animAttr.toPos.y - yy)* dt / animAttr.duration;

	if (animAttr.isSizeAnim)
		dw = (animAttr.toSize.width - ww)* dt / animAttr.duration,
		dh = (animAttr.toSize.height - hh)* dt / animAttr.duration;

	isAnimInit = false;

	animFunc = [animAttr, dx, dy, dw, dh, dt, xx, yy, ww, hh, tt, completeCallback, this](bool& isInit) {

		static float x, y, w, h, t;	static Pos lastPos;	static Size lastSize;
		if (!isInit)
			x = xx, y = yy, w = ww, h = hh, t = tt, lastPos = canvas.getPos(), lastSize = canvas.getSize(), isInit = true;

		t += dt;
		bool opcode = false;

		if (t < animAttr.duration)
		{
			if (animAttr.isPosAnim)
			{
				x += dx, y += dy;

				if (lastPos.x != int(x) || lastPos.y != int(y))
				{
					canvas.setPos({ int(x),int(y) }), lastPos = canvas.getPos();
					opcode |= true;
				}
			}

			if (animAttr.isSizeAnim)
			{
				w += dw, h += dh;

				if (lastSize.width != int(x) || lastSize.height != int(y))
				{
					canvas.setSize({ int(w),int(h) }), lastSize = canvas.getSize();
					opcode |= true;
				}
			}
		}
		else
		{
			if (completeCallback) setAnimCallback(completeCallback);
			animFunc = nullptr;
		}

		return opcode;
	};
}

/* Label ****************************************************/

Label::Label(const wchar_t * cstr, Pos4 pos4, const Pos& parentPos, const Size& parentSize) :str(cstr), Elements(ElementsType::Button)
{
	// set values
	canvas = Canvas(pos4, { int(str.length()),1 }, parentPos, parentSize);

	// update canvas
	Elements::flush();
}

/* Button ****************************************************/

Button::Button(const wchar_t * cstr, Pos4 pos4, const Pos& parentPos, const Size& parentSize, bool isFrame, Size padding) :str(cstr), Elements(ElementsType::Button)
{
	// set values
	canvas = Canvas(pos4, { int(str.length() + padding.width * 2),padding.height * 2 + 1 }, parentPos, parentSize, isFrame, L'╳');

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
		Elements::flush(L'╬');
	else
		Elements::flush();

	bool opcode = false;
	if (mouseHoverCallback)
		opcode |= mouseHoverCallback();

	return true;
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
		opcode |= mouseClkCallback();
	return opcode;
}

/* Inputbox ****************************************************/

Inputbox::Inputbox(const wchar_t * cstr, Pos4 pos4, const Pos& parentPos, const Size& parentSize, int len, bool isFrame) :str(cstr), originStr(cstr), len(len), Elements(ElementsType::Inputbox)
{
	// set values
	canvas = Canvas(pos4, { len + 2,3 }, parentPos, parentSize, isFrame, L' ');

	// update canvas
	flush();

	// pollingCallback
	int timeCount = 0;
	setPollingCallback([&timeCount, this]() {
		timeCount = ++timeCount % int(MY_UPDATE_FREQ);

		if (isFlash && (timeCount == 0 || timeCount == int(MY_UPDATE_FREQ / 2)))
		{
			isWhite = !isWhite, isWhite ? canvas.flush(L'█') : flush();
			return true;
		}

		return false;
	});
}

void Inputbox::flush_impl(wchar_t flushChar)
{
	flushChar ? canvas.flush(flushChar) : canvas.flush();


	std::wstring cstr = L"  " + str;
	canvas.line(1, 1, cstr.c_str(), int(cstr.length()));
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
			opcode |= keyPrsCallback(e);
	}
	
	return opcode;
}

/* Image ****************************************************/

Image::Image(CharImage charImage, const Pos& parentPos, const Size& parentSize, bool isFrame) :charImage(charImage), Elements(ElementsType::Image)
{
	// set values
	canvas = Canvas(charImage.pos4, charImage.size, parentPos, parentSize, isFrame);

	// update canvas
	Elements::flush();
}

/* Paragraph ****************************************************/

Paragraph::Paragraph(const wchar_t * cstr, Pos4 pos4, Size size, const Pos& parentPos, const Size & parentSize,
	TextAlign textAlign, bool isFrame, Size padding, wchar_t flushChar)
	: str(cstr), textAlign(textAlign), padding(padding), Elements(ElementsType::Paragraph)
{
	// set values
	canvas = Canvas(pos4, size, parentPos, parentSize, isFrame, flushChar);

	// update canvas
	Elements::flush();
}

void Paragraph::flush_impl(wchar_t flushChar)
{
	flushChar ? canvas.flush(flushChar) : canvas.flush();

	int i = padding.height, start = 0, end, breakInd;
	do
	{
		if (str[start] == L'\n')
		{
			start++, i++;
			continue;
		}

		end = start + canvas.getSize().width - 2 * padding.width > str.length() ?
			int(str.length()) : start + canvas.getSize().width - 2 * padding.width;

		breakInd = int(str.find(L'\n', start + 1));
		if (breakInd + 1 && breakInd < end)
			end = breakInd;

		if (textAlign == TextAlign::left)
			canvas.line(padding.width, i++, str.substr(start, end - start));
		else if (textAlign == TextAlign::center)
			canvas.lineCenter(canvas.getSize().width / 2, i++, str.substr(start, end - start));
		else
			canvas.line(canvas.getSize().width - (end - start) - padding.width, i++, str.substr(start, end - start));

		start = end;
		if (end == breakInd)
			i--;
	} while (end != str.length() && i < canvas.getSize().height - padding.height);
}
