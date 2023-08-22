#pragma once

// useful elements under Canvas

// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// !!!!  std::function only can use var in same (derived) class, or cannot access    !!!!!!!!
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

#include <functional>

#include "Window/Event.h"
#include "Canvas.h"

#include "Animate.h"

/* Elements ****************************************************/

struct WindowMouseInfo;

enum class ElementsType { none, Label, Button, Inputbox, Image, Paragraph, Div };
struct ElementsInfo
{
	enum InfoType { none, active, cancel };

	InfoType mousePressed = none, mouseHover = none, mouseClick = none;
	bool isVisible = true;
};
class Elements
{
public:
	Elements(ElementsType type, Canvas& parent) : type(type), parent(parent) {}
	virtual ~Elements() {}

	friend Window;

	template <typename T> inline T& get() {	return dynamic_cast<T&>(*this); }

	inline void flush(wchar_t flushChar = 0) { flush_impl(flushChar), isNeedUpdate = true; }              // (isNeedUpdate = true) already
	inline bool onMousePrs(MousePrsEvent e) { return _onMousePrs(e) ? onMousePrs_impl(e) : false; }	      // be sure to set isNeedUpdate
	inline bool onMouseMove(MouseMoveEvent e) { return  _onMouseMove(e) ? onMouseMove_impl(e) : false; }  // be sure to set isNeedUpdate
	inline bool onMouseRls(MouseRlsEvent e) { return _onMouseRls(e) ? onMouseRls_impl(e) : false; }	      // be sure to set isNeedUpdate
	inline bool onKeyPrs(KeyPrsEvent e) { return onKeyPrs_impl(e); }	                                  // be sure to set isNeedUpdate
	inline void onWindowResize(WindowResizeEvent e) { _onWindowResize(e), onWindowResize_impl(e); }       // be sure to set isNeedUpdate
	Status onPollingUpdate(WindowMouseInfo& mouseInfo);

	void animate(Animate animateAttr, std::function<void()> callback = nullptr);
	inline void isAnimatePause(bool b) { anim.status = b ? Animate::pause : Animate::play; }
	inline void toggleAnimateStatus() { anim.status = anim.status == Animate::play ? Animate::pause : Animate::play; }

	inline Canvas& getCanvas() { return canvas; }
	inline Animate& getAnimate() { return anim; }
	inline const bool& getVisible() { return info.isVisible; }
	inline const ElementsType& getType() { return type; }
	inline const unsigned int& getId() { return id; }
	inline const unsigned int& getZindex() { return zindex; }
	inline const ElementsInfo& getInfo() { return info; }

	inline void setPos4(Pos4 pos4) { canvas.setPos4(pos4, parent); }
	inline void setSize2(Size2 size2) { canvas.setSize2(size2, parent), flush(); }
	inline void setId(const unsigned int& i) { id = i; }
	inline void setVisible(bool b) { info.isVisible = b; }
	inline void setZindex(const unsigned int& i) { zindex = i, isForceZindex = true; }

	inline bool pollingZindex() { bool b = isForceZindex; isForceZindex = false; return b; }

protected:
	virtual void flush_impl(wchar_t flushChar) = 0;
	virtual bool onMouseMove_impl(MouseMoveEvent& e) { return false; }
	virtual bool onMousePrs_impl(MousePrsEvent& e) { return false; }
	virtual bool onMouseRls_impl(MouseRlsEvent& e) { return false; }
	virtual bool onKeyPrs_impl(KeyPrsEvent& e) { return false; }
	virtual void onWindowResize_impl(WindowResizeEvent& e) {}

	inline void setPollingCallback(std::function<bool()> func) { pollingCallback = func; }

	Canvas& parent;
	Canvas canvas;
	ElementsInfo info;  // for Event dispatch use
	bool isNeedUpdate = false;
private:
	ElementsType type;
	unsigned int id, zindex;
	bool isForceZindex = false;
	
	std::function<bool()> pollingCallback = nullptr;

	// run default
	bool _onMouseMove(MouseMoveEvent& e);
	bool _onMousePrs(MousePrsEvent& e);
	bool _onMouseRls(MouseRlsEvent& e);
	void _onWindowResize(WindowResizeEvent& e);

	Animate anim;
	std::function<bool()> animFunc = nullptr;
	std::function<void()> animCallback = nullptr;
	inline void setAnimCallback(std::function<void()> func) { animCallback = func; }
};

/* Label ****************************************************/

class Label : public Elements
{
public:
	Label(const wchar_t * cstr, Pos4 pos4, Window& parent, bool isWhitespace = true);

	inline void setString(std::wstring _str) { str = _str; flush(); }
private:
	std::wstring str;
	bool isWhitespace;

	virtual void flush_impl(wchar_t flushChar) override { canvas.setSize2widthValue(int(str.length()), parent), canvas.canvasCenterLine(str, isWhitespace); }
};

/* Button ****************************************************/

class Button :public Elements
{
public:
	Button(const wchar_t * cstr, Pos4 pos4, Window& parent, bool isFrame, Size padding = Size({ 6,2 }));

	inline void setString(std::wstring _str, wchar_t flushChar = 0) { str = _str; flush(flushChar); }
	
	void onhover(std::function<bool()> func) { mouseHoverCallback = func; }
	void onclick(std::function<bool()> func) { mouseClkCallback = func; }
private:
	std::wstring str;
	
	virtual void flush_impl(wchar_t flushChar) override;
	virtual bool onMouseMove_impl(MouseMoveEvent& e) override;
	virtual bool onMousePrs_impl(MousePrsEvent& e) override;
	virtual bool onMouseRls_impl(MouseRlsEvent& e) override;

	std::function<bool()> mouseHoverCallback = nullptr;
	std::function<bool()> mouseClkCallback = nullptr;
};

/* Inputbox ****************************************************/

class Inputbox :public Elements
{
public:
	Inputbox(const wchar_t * cstr, Pos4 pos4, Window& parent, int len, bool isFrame);
	~Inputbox() { isFlash = false; }

	void onkey(std::function<bool(KeyPrsEvent&)> func) { keyPrsCallback = func; }

	inline void setString(std::wstring _str, wchar_t flushChar = 0) { str = _str, originStr = _str; flush(flushChar); }
	inline std::wstring& getString() { return str; }
private:
	std::wstring str, originStr;
	int len;
	bool isFocus = false, isFlash = false, isWhite = false;
	short timeCount = 0;

	virtual void flush_impl(wchar_t flushChar) override;
	virtual bool onMouseMove_impl(MouseMoveEvent& e) override;
	virtual bool onMouseRls_impl(MouseRlsEvent& e) override; 
	virtual bool onKeyPrs_impl(KeyPrsEvent& e) override;

	std::function<bool(KeyPrsEvent&)> keyPrsCallback = nullptr;
};

/* Image ****************************************************/

class Image : public Elements
{
public:
	Image(CharImage charImage, Window& parent, bool isFrame = false, bool isWhitespace = false);
private:
	CharImage charImage;
	bool isWhitespace;

	virtual void flush_impl(wchar_t flushChar) override { canvas.renderCharImage(charImage, isWhitespace); }
};

/* Paragraph ****************************************************/

class Paragraph : public Elements
{
public:
	Paragraph(const wchar_t * cstr, Pos4 pos4, Size2 size2, Window& parent,
		TextAlign textAlign = TextAlign::left, bool isFrame = false, Size padding = Size({ 2,1 }), bool isWhitespace = true, wchar_t flushChar = L'•');

	inline void setString(std::wstring _str, wchar_t flushChar = 0) { str = _str; flush(flushChar); }
private:
	std::wstring str;
	TextAlign textAlign;
	Size padding;
	bool isWhitespace;

	virtual void flush_impl(wchar_t flushChar) override;
};

/* Div ****************************************************/

class Div : public Elements
{
public:
	Div(Pos4 pos4, Size2 size2, Window& parent, bool isFrame = false, Size padding = Size({ 0,0 }), wchar_t flushChar = TRANSPARENT_WCHAR);

private:
	Size padding;

	virtual void flush_impl(wchar_t flushChar) override { }
};