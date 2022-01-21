#pragma once

// useful elements under Canvas

// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// !!!!  std::function only can use var in same (derived) class, or cannot access    !!!!!!!!
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

#include <functional>

#include "Window/Event.h"
#include "Canvas.h"

#define CREATE_ELEMENTS(className , name, onclick) dynamic_cast<T&>(*this)

class App;
class Window;

struct Animate
{
	Pos toPos;
	Size toSize;
	bool isPosAnim = false, isSizeAnim = false;
	float duration;
	enum Easing { linear, swing, ritard, accel };
	Easing easing;

	Animate(Pos pos, Size size, float duration, Easing easing = linear) :toPos(pos), toSize(size), duration(duration), easing(easing) { isPosAnim = true, isSizeAnim = true; }
	Animate(Pos pos, float duration, Easing easing = linear) :toPos(pos), duration(duration), easing(easing) { isPosAnim = true; }
	Animate(Size size, float duration, Easing easing = linear) :toSize(size), duration(duration), easing(easing) { isSizeAnim = true; }
};

enum class InfoType { None, Active, Cancel };
struct ElementsInfo
{
	InfoType mousePressed = InfoType::None, mouseHover = InfoType::None, mouseClick = InfoType::None;
	bool isVisible = true;
};

enum class ElementsType { None, Label, Button, Inputbox, Image, Paragraph };
class Elements
{
public:
	Elements(ElementsType type) :type(type) {}
	virtual ~Elements() {}

	friend Window;

	template <typename T> inline T& get() {	return dynamic_cast<T&>(*this); }

	inline void flush(wchar_t flushChar = 0) { flush_impl(flushChar), isNeedUpdate = true; }  // isNeedUpdate = true already
	inline bool onMouseMove(MouseMoveEvent& e) { return  _onMouseMove(e) ? onMouseMove_impl(e) : false; }  // be sure to set isNeedUpdate
	inline bool onMousePrs(MousePrsEvent& e) { return _onMousePrs(e) ? onMousePrs_impl(e) : false; }	   // be sure to set isNeedUpdate
	inline bool onMouseRls(MouseRlsEvent& e) { return _onMouseRls(e) ? onMouseRls_impl(e) : false; }	   // be sure to set isNeedUpdate
	inline bool onKeyPrs(KeyPrsEvent& e) { return onKeyPrs_impl(e); }	                                   // be sure to set isNeedUpdate

	inline bool pendingUpdate() { bool b = isNeedUpdate; isNeedUpdate = false; return b; }
	bool onPollingUpdate(bool& isForceWindowRefresh);

	void animate(Animate animateAttr, std::function<void()> completeCallback = nullptr);
	inline void setAnimCallback(std::function<void()> func) { animCallback = func; }

	inline void setPos4(Pos4 _pos4) { canvas.setPos4(_pos4); }
	inline void setVisible(bool b) { info.isVisible = b; }
	inline const bool& getVisible() { return info.isVisible; }
	inline Canvas& getCanvas() { return canvas; }
	inline const ElementsType& getType() { return type; }
	inline const unsigned int& getId() { return id; }
	inline const ElementsInfo& getInfo() { return info; }

	inline void setZindex(const unsigned int& i) { zindex = i, isForceZindex = true; }
	inline bool pollingZindex() { bool b = isForceZindex; isForceZindex = false; return b; }
protected:
	virtual void flush_impl(wchar_t flushChar) = 0;
	virtual bool onMouseMove_impl(MouseMoveEvent& e) { return false; }
	virtual bool onMousePrs_impl(MousePrsEvent& e) { return false; }
	virtual bool onMouseRls_impl(MouseRlsEvent& e) { return false; }
	virtual bool onKeyPrs_impl(KeyPrsEvent& e) { return false; }

	inline void setPollingCallback(std::function<bool()> func) { pollingCallback = func; }

	Canvas canvas;
	ElementsInfo info;
	bool isNeedUpdate = false;
private:
	ElementsType type;
	unsigned int id, zindex;
	inline void setId(const unsigned int& i) { id = i; }
	bool isForceZindex = false;

	std::function<bool()> pollingCallback = nullptr;

	// preset
	bool _onMouseMove(MouseMoveEvent& e);
	bool _onMousePrs(MousePrsEvent& e);
	bool _onMouseRls(MouseRlsEvent& e);

	bool isAnimInit = false;
	std::function<bool(bool&)> animFunc = nullptr;
	std::function<void()> animCallback = nullptr;
};

/* Label ****************************************************/

class Label : public Elements
{
public:
	Label(const wchar_t * cstr, Pos4 pos4, const Pos& parentPos, const Size& parentSize);

	inline void setString(std::wstring _str) { str = _str; flush(); }
private:
	std::wstring str;

	virtual void flush_impl(wchar_t flushChar) override { canvas.setWidth(int(str.length())), canvas.canvasCenterLine(str); }
};

/* Button ****************************************************/

class Button :public Elements
{
public:
	Button(const wchar_t * cstr, Pos4 pos4, const Pos& parentPos, const Size& parentSize, bool isFrame, Size padding = Size({ 6,2 }));

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
	Inputbox(const wchar_t * cstr, Pos4 pos4, const Pos& parentPos, const Size& parentSize, int len, bool isFrame);
	~Inputbox() { isFlash = false; }

	void onkey(std::function<bool(KeyPrsEvent&)> func) { keyPrsCallback = func; }

	inline void setString(std::wstring _str, wchar_t flushChar = 0) { str = _str, originStr = _str; flush(flushChar); }
	inline std::wstring& getString() { return str; }
private:
	std::wstring str, originStr;
	int len;
	bool isFocus = false, isFlash = false, isWhite = false;

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
	Image(CharImage charImage, const Pos& parentPos, const Size& parentSize, bool isFrame = false);
private:
	CharImage charImage;

	virtual void flush_impl(wchar_t flushChar) override { canvas.setCharImage(charImage); }
};

/* Paragraph ****************************************************/

class Paragraph : public Elements
{
public:
	Paragraph(const wchar_t * cstr, Pos4 pos4, Size size, const Pos& parentPos, const Size& parentSize,
		TextAlign textAlign = TextAlign::left, bool isFrame = false, Size padding = Size({ 2,1 }), wchar_t flushChar = L'•');

	inline void setString(std::wstring _str, wchar_t flushChar = 0) { str = _str; flush(flushChar); }
private:
	std::wstring str;
	TextAlign textAlign;
	Size padding;

	virtual void flush_impl(wchar_t flushChar) override;
};