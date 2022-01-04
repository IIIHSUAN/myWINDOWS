#pragma once

// useful elements under Canvas

#include <functional>

#include "Window/Event.h"
#include "Canvas.h"

#define CREATE_ELEMENTS(className , name, onclick) dynamic_cast<T&>(*this)

class App;
class Window;

enum class ElementsType
{
	None, Button, Label
};

class Elements
{
public:
	Elements(ElementsType type) :type(type) {}

	friend Window;

	template <typename T> inline T& get() {	return dynamic_cast<T&>(*this); }

	inline void flush(wchar_t flushChar = 0) { flush_impl(flushChar); }
	inline bool onMouseMove(MouseMoveEvent& e) { return onMouseMove_impl(e); }
	inline bool onMousePrs(MousePrsEvent& e) { return onMousePrs_impl(e); }
	inline bool onMouseRls(MouseRlsEvent& e) { return onMouseRls_impl(e); }

	inline void setPos(Pos _pos) { canvas.setPos(_pos); }
	inline void setVisible(bool b) { isVisible = b; }
	inline const bool& getVisible() { return isVisible; }
	inline Canvas& getCanvas() { return canvas; }
	inline const ElementsType& getType() { return type; }
	inline const unsigned int& getId() { return id; }
protected:
	virtual void flush_impl(wchar_t flushChar) = 0;
	virtual bool onMouseMove_impl(MouseMoveEvent& e) = 0;
	virtual bool onMousePrs_impl(MousePrsEvent& e) = 0;
	virtual bool onMouseRls_impl(MouseRlsEvent& e) = 0;

	Canvas canvas;
private:
	ElementsType type;
	bool isMousePrs = false, isMouseMove = false, isVisible = true;
	unsigned int id;

	inline void setId(const unsigned int& i) { id = i; }
};

/* Button ****************************************************/

class Button :public Elements
{
public:
	Button(const wchar_t * cstr, Pos pos, bool isFrame);
		
	inline void setString(std::wstring _str, wchar_t flushChar = 0) { str = _str; flush(flushChar); }
	
	void onhover(std::function<bool(Button&)> func) { mouseMoveCallback = func; }
	void onpress(std::function<bool(Button&)> func) { mousePrsCallback = func; }
	void onclick(std::function<bool(Button&)> func) { mouseRlsCallback = func; }
private:
	std::wstring str, originStr;
	bool isHover = false, isHoverFunc = false, isPrs = false;

	virtual void flush_impl(wchar_t flushChar) override;
	virtual bool onMouseMove_impl(MouseMoveEvent& e) override;
	virtual bool onMousePrs_impl(MousePrsEvent& e) override;
	virtual bool onMouseRls_impl(MouseRlsEvent& e) override;

	std::function<bool(Button&)> mouseMoveCallback = nullptr;
	std::function<bool(Button&)> mousePrsCallback = nullptr;
	std::function<bool(Button&)> mouseRlsCallback = nullptr;
};

/* Label ****************************************************/
