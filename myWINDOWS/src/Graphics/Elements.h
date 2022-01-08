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

enum class InfoType
{
	None, Active, Cancel
};
struct ElementsInfo
{
	InfoType mousePressed = InfoType::None, mouseHover = InfoType::None, mouseClick = InfoType::None;
	bool isVisible = true;
};

enum class ElementsType
{
	None, Button, Inputbox, Panel
};
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

	inline void setPos(Pos _pos) { canvas.setPos(_pos); }
	inline void setVisible(bool b) { info.isVisible = b; }
	inline const bool& getVisible() { return info.isVisible; }
	inline Canvas& getCanvas() { return canvas; }
	inline const ElementsType& getType() { return type; }
	inline const unsigned int& getId() { return id; }
	inline const ElementsInfo& getInfo() { return info; }

	inline bool pendingUpdate() { bool b = isNeedUpdate; isNeedUpdate = false; return b; }
protected:
	virtual void flush_impl(wchar_t flushChar);
	virtual bool onMouseMove_impl(MouseMoveEvent& e) { return false; }
	virtual bool onMousePrs_impl(MousePrsEvent& e) { return false; }
	virtual bool onMouseRls_impl(MouseRlsEvent& e) { return false; }
	virtual bool onKeyPrs_impl(KeyPrsEvent& e) { return false; }

	Canvas canvas;
	ElementsInfo info;
	bool isNeedUpdate = false;
private:
	ElementsType type;
	unsigned int id;

	inline void setId(const unsigned int& i) { id = i; }

	// preset
	bool _onMouseMove(MouseMoveEvent& e);
	bool _onMousePrs(MousePrsEvent& e);
	bool _onMouseRls(MouseRlsEvent& e);
};

/* Button ****************************************************/

class Button :public Elements
{
public:
	Button(const wchar_t * cstr, Pos pos, bool isFrame);
		
	inline void setString(std::wstring _str, wchar_t flushChar = 0) { str = _str; flush(flushChar); }
	
	void onhover(std::function<bool(Button&)> func) { mouseHoverCallback = func; }
	void onclick(std::function<bool(Button&)> func) { mouseClkCallback = func; }
private:
	std::wstring str;
	
	virtual void flush_impl(wchar_t flushChar) override;
	virtual bool onMouseMove_impl(MouseMoveEvent& e) override;
	virtual bool onMousePrs_impl(MousePrsEvent& e) override;
	virtual bool onMouseRls_impl(MouseRlsEvent& e) override;

	std::function<bool(Button&)> mouseHoverCallback = nullptr;
	std::function<bool(Button&)> mouseClkCallback = nullptr;
};

/* Inputbox ****************************************************/

class Inputbox :public Elements
{
public:
	Inputbox(const wchar_t * cstr, Pos pos, int len, bool isFrame);
	~Inputbox() { isFlash = false; }

	void onkey(std::function<bool(KeyPrsEvent& e, std::wstring&)> func) { keyPrsCallback = func; }

	inline void setString(std::wstring _str, wchar_t flushChar = 0) { str = _str; flush(flushChar); }
private:
	std::wstring str, originStr;
	int len;
	bool isFocus = false, isFlash = false;

	virtual void flush_impl(wchar_t flushChar) override;
	virtual bool onMouseMove_impl(MouseMoveEvent& e) override;
	virtual bool onMouseRls_impl(MouseRlsEvent& e) override; 
	virtual bool onKeyPrs_impl(KeyPrsEvent& e) override;

	std::function<bool(KeyPrsEvent& e, std::wstring&)> keyPrsCallback = nullptr;
};

/* Panel ****************************************************/

class Panel :public Elements
{
public:
	Panel(Pos pos, Size size, bool isFrame, CharImage* charImage = 0);

	inline void setBackground(CharImage* _charImage) { charImage = _charImage, Elements::flush(); }

	void onhover(std::function<bool(Panel& p, MouseMoveEvent&)> func) { mouseHoverCallback = func; }
	void onclick(std::function<bool(Panel& p, MouseRlsEvent&)> func) { mouseClkCallback = func; }
private:
	CharImage* charImage;

	virtual void flush_impl(wchar_t flushChar) override;
	virtual inline bool onMouseMove_impl(MouseMoveEvent& e) override { return mouseHoverCallback ? mouseHoverCallback(*this, e) : false; }
	virtual inline bool onMouseRls_impl(MouseRlsEvent& e) override { return (mouseClkCallback && info.mouseClick == InfoType::Active) ? mouseClkCallback(*this, e) : false; }

	std::function<bool(Panel&, MouseMoveEvent&)> mouseHoverCallback = nullptr;
	std::function<bool(Panel&, MouseRlsEvent&)> mouseClkCallback = nullptr;
};