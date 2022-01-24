#include "Elements.h"

#include <System/System.h>

#include <Windows.h>
#include <thread>

/* Elements ****************************************************/

void Elements::flush_impl(wchar_t flushChar)
{
	if (flushChar)
		canvas.flush(flushChar);
	else
		canvas.flush();
}

bool Elements::_onMouseMove(MouseMoveEvent & e)
{
	Pos pos = canvas.getPos();
	Size size = canvas.getSize();
	e.setPos({ e.getMouseX() - pos.x , e.getMouseY() - pos.y });

	bool isCallback = false;
	if (e.getMouseX() >= 0 && e.getMouseX() < size.width && e.getMouseY() >= 0 && e.getMouseY() < size.height)
		info.mouseHover = ElementsInfo::active, isCallback = true;
	else if (info.mouseHover == ElementsInfo::active)
		info.mouseHover = ElementsInfo::cancel, isCallback = true;
	else if (info.mouseHover == ElementsInfo::cancel)
		info.mouseHover = ElementsInfo::none;

	if (info.mouseClick == ElementsInfo::active)
		info.mouseClick = ElementsInfo::cancel, isCallback = true;
	else if (info.mouseClick == ElementsInfo::cancel)
		info.mouseClick = ElementsInfo::none;

	return isCallback;
}

bool Elements::_onMousePrs(MousePrsEvent & e)
{
	Pos& pos = canvas.getPos();
	Size& size = canvas.getSize();
	e.setPos({ e.getMouseX() - pos.x , e.getMouseY() - pos.y });

	bool isCallback = false;
	if (e.getMouseX() >= 0 && e.getMouseX() < size.width && e.getMouseY() >= 0 && e.getMouseY() < size.height)
		info.mousePressed = ElementsInfo::active, isCallback = true;

	return isCallback;
}

bool Elements::_onMouseRls(MouseRlsEvent & e)
{
	Pos& pos = canvas.getPos();
	Size& size = canvas.getSize();
	e.setPos({ e.getMouseX() - pos.x , e.getMouseY() - pos.y });

	bool isCallback = false;
	if (e.getMouseX() >= 0 && e.getMouseX() < size.width&&e.getMouseY() >= 0 && e.getMouseY() < size.height
		&& info.mousePressed == ElementsInfo::active)
		info.mouseClick = ElementsInfo::active, isCallback = true;

	if (info.mousePressed == ElementsInfo::active)
		info.mousePressed = ElementsInfo::cancel, isCallback = true;
	else if (info.mousePressed == ElementsInfo::cancel)
		info.mousePressed = ElementsInfo::none;

	return isCallback;
}

void Elements::_onWindowResize(WindowResizeEvent & e)
{
	if (animFunc)
	{
		// size
		anim.startSize.width = canvas.convertSize2width(anim.startSize2, parent), anim.startSize.height = canvas.convertSize2height(anim.startSize2, parent);
		anim.endSize.width = canvas.convertSize2width(anim.endSize2, parent), anim.endSize.height = canvas.convertSize2height(anim.endSize2, parent);
		// pos
		anim.startPos.x = canvas.convertPos4h(anim.startPos4, parent), anim.startPos.y = canvas.convertPos4v(anim.startPos4, parent);
		anim.endPos.x = canvas.convertPos4h(anim.endPos4, parent), anim.endPos.y = canvas.convertPos4v(anim.endPos4, parent);
	}
	else
		setSize2(canvas.getSize2()), setPos4(canvas.getPos4());
}

bool Elements::onPollingUpdate(bool& isForceWindowRefresh)
{
	bool b = isNeedUpdate;
	if (pollingCallback) b |= pollingCallback();
	if (animFunc && anim.status == Animate::play && animFunc()) 
		onMouseMove(MouseMoveEvent(Mouse::get().X, Mouse::get().Y, Mouse::get().X, Mouse::get().Y,false)),  // virtual MouseMoveEvent
		isForceWindowRefresh |= true;
	if (animCallback) animCallback(), animCallback = nullptr;
	isNeedUpdate = false; return b;
}

void Elements::animate(Animate _anim, std::function<void()> callback)
{
	float sleepTime = anim.sleepTime;
	anim = _anim;
	anim.sleepTime = sleepTime;
	anim.dt = System::get().getPollingPeriod();
	// size
	anim.startSize2 = canvas.getSize2();
	anim.startSize.width = canvas.convertSize2width(anim.startSize2, parent), anim.startSize.height = canvas.convertSize2height(anim.startSize2, parent);
	anim.endSize.width = canvas.convertSize2width(anim.endSize2, parent), anim.endSize.height = canvas.convertSize2height(anim.endSize2, parent);
	// pos
	anim.startPos4 = canvas.getPos4();
	anim.startPos.x = canvas.convertPos4h(anim.startPos4, parent), anim.startPos.y = canvas.convertPos4v(anim.startPos4, parent);
	anim.endPos.x = canvas.convertPos4h(anim.endPos4, parent), anim.endPos.y = canvas.convertPos4v(anim.endPos4, parent);
	anim.callback = callback;

	animFunc = [this]() {
		if (anim.sleepTime > 0)
		{
			anim.sleepTime -= anim.dt;
			return false;
		}

		bool opcode = false;
		
		anim.time += anim.dt;
		float ratio = anim.easingFunc ? anim.easingFunc(anim.time / anim.duration) : anim.time / anim.duration;

		if (anim.time < anim.duration - anim.dt)
		{

			if (anim.isSizeAnim)
			{
				anim.w = anim.startSize.width * (1 - ratio) + anim.endSize.width * ratio;
				anim.h = anim.startSize.height * (1 - ratio) + anim.endSize.height * ratio;

				if (anim.lastSize.width != int(anim.w) || anim.lastSize.height != int(anim.h))
				{
					setSize2({ int(anim.w),int(anim.h) });

					if (anim.isPosAnim)
					{
						anim.startPos.x = canvas.convertPos4h(anim.startPos4, parent), anim.startPos.y = canvas.convertPos4v(anim.startPos4, parent);
						anim.endPos.x = canvas.convertPos4h(anim.endPos4, parent), anim.endPos.y = canvas.convertPos4v(anim.endPos4, parent);
					}

					anim.lastSize.width = int(anim.w), anim.lastSize.height = int(anim.h);
					opcode |= true;
				}
			}

			if (anim.isPosAnim)
			{
				anim.x = anim.startPos.x * (1 - ratio) + anim.endPos.x * ratio;
				anim.y = anim.startPos.y * (1 - ratio) + anim.endPos.y * ratio;

				if (anim.lastPos.x != int(anim.x) || anim.lastPos.y != int(anim.y))
				{
					if (!anim.isSetCanvasPos4)
						setPos4({ Left(int(anim.x)),Top(int(anim.y)) }), anim.isSetCanvasPos4 = true;
					else
						canvas.setPos4hValue(int(anim.x), parent), canvas.setPos4vValue(int(anim.y), parent);

					anim.lastPos.x = int(anim.x), anim.lastPos.y = int(anim.y);
					opcode |= true;
				}
			}
		}
		else  // return 
		{
			if (anim.isSizeAnim) setSize2(anim.endSize2);
			if (anim.isPosAnim)	setPos4(anim.endPos4);
			opcode |= true;

			if (anim.callback) setAnimCallback(anim.callback);
			animFunc = nullptr;
		}

		return opcode;
	};
}

/* Label ****************************************************/

Label::Label(const wchar_t * cstr, Pos4 pos4, Window& window) 
	: str(cstr), Elements(ElementsType::Button, window.getCanvas())
{
	// set values
	canvas = Canvas(pos4, { int(str.length()),1 }, parent);

	// update canvas
	Elements::flush();
}

/* Button ****************************************************/

Button::Button(const wchar_t * cstr, Pos4 pos4, Window& window, bool isFrame, Size padding)
	: str(cstr), Elements(ElementsType::Button, window.getCanvas())
{
	// set values
	canvas = Canvas(pos4, { int(str.length() + padding.width * 2),padding.height * 2 + 1 }, parent, isFrame, L'╳');

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
	if (info.mouseHover == ElementsInfo::active)
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
	if (info.mousePressed == ElementsInfo::active)
		Elements::flush(L'|');

	return false;
}

bool Button::onMouseRls_impl(MouseRlsEvent & e)
{
	bool opcode = false;
	if (info.mousePressed == ElementsInfo::cancel)
		Elements::flush();

	if (info.mouseClick == ElementsInfo::active && mouseClkCallback)
		opcode |= mouseClkCallback();
	return opcode;
}

/* Inputbox ****************************************************/

Inputbox::Inputbox(const wchar_t * cstr, Pos4 pos4, Window& window, int len, bool isFrame) 
	:str(cstr), originStr(cstr), len(len), Elements(ElementsType::Inputbox, window.getCanvas())
{
	// set values
	canvas = Canvas(pos4, { len + 2,3 }, parent, isFrame, L' ');

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
	canvas.line(1, 1, cstr.c_str(), min(int(cstr.length()), len-2));
}

bool Inputbox::onMouseMove_impl(MouseMoveEvent & e)
{
	if (info.mouseHover == ElementsInfo::active && !isFlash)
		isFlash = true,	isWhite = false;
	else if (info.mouseHover == ElementsInfo::cancel)
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
	if (info.mouseClick == ElementsInfo::active)
		originStr = str, setString(L""), isFocus = true;

	return false;
}

bool Inputbox::onKeyPrs_impl(KeyPrsEvent & e)
{
	bool opcode = false;
	if (isFocus)
	{
		isFlash = false;

		if(e.getKey() == KeySet::enter)
			originStr = str;
		else if (e.getKey() == KeySet::backspace)
			str = str.substr(0, str.length() - 1);
		else
			str += e.getChar();
		flush();

		if(keyPrsCallback)
			opcode |= keyPrsCallback(e);
	}
	
	return opcode;
}

/* Image ****************************************************/

Image::Image(CharImage charImage, Window& window, bool isFrame) :charImage(charImage), Elements(ElementsType::Image, window.getCanvas())
{
	// set values
	canvas = Canvas(charImage.pos4, { charImage.size.width - 1,charImage.size.height }, parent, isFrame);

	// update canvas
	Elements::flush();
}

/* Paragraph ****************************************************/

Paragraph::Paragraph(const wchar_t * cstr, Pos4 pos4, Size2 size2, Window& window,
	TextAlign textAlign, bool isFrame, Size padding, wchar_t flushChar)
	: str(cstr), textAlign(textAlign), padding(padding), Elements(ElementsType::Paragraph, window.getCanvas())
{
	// set values
	canvas = Canvas(pos4, size2, parent, isFrame, flushChar);

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
