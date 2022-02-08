#pragma once

#include <functional>
#include <list>

#include "Event.h"
#include "Graphics/Canvas.h"
#include "Graphics/Elements.h"

class App;
class Elements;

#define push_elements(ele_ptr, ele_entity) ele_ptr.reset(new ele_entity), pushElements(ele_ptr)
#define FRONT(ind) getElementsListSize() - 1 - ind

struct WindowMouseInfo
{
	unsigned int handledElementInd = 0;  // for Element animate use
	bool isPrsFrame = false, isPrsCorner = false, isFocus = false;
	Pos last;
	inline void setLast(Pos&& pos) { last = pos; }
};

enum class WindowCollection { unknown, default, devTool };
class Window
{
public:
	Window(std::wstring _name, Pos _pos, Size _size, const wchar_t flushChar = WHITESPACE_WCHAR, WindowCollection window = WindowCollection::default);

	friend class App;
	friend class Elements;

	inline const int& getX() { return pos.x; }
	inline const int& getY() { return pos.y; }
	inline const Pos& getPos() { return pos; }
	inline const Size& getSize() { return size; }
	inline Canvas& getCanvas() { return canvas; }
	inline const WindowCollection& getType() { return window; }
	inline const int& getId() { return id; }
	inline std::list<std::shared_ptr<Elements>>& getElementsList() { return elementsList; }
	inline const bool& getIsRun() { return isRun; }

	inline void setTitle(std::wstring& str) { title = str; }
	inline void setId(unsigned int i) { id = i; }

	inline void setRecieveCallback	 (std::function<void(std::string)> func)		{ recvCallback = func; };
	inline void setMouseMoveCallback (std::function<void(MouseMoveEvent)> func)		{ mouseMoveCallback = func; };
	inline void setMousePrsCallback	 (std::function<void(MousePrsEvent)> func)		{ mousePrsCallback = func; };
	inline void setKeyPrsCallback	 (std::function<bool(KeyPrsEvent)> func)		{ keyPrsCallback = func; };
	inline void setResizeCallback	 (std::function<void(WindowResizeEvent)> func)	{ resizeCallback = func; };
	inline void setPollingCallback	 (std::function<Status(void)> func)				{ pollingCallback = func; };

	Status pollingUpdate();

	void setIsNeedUpdate(bool b) { isNeedUpdate |= b; }
protected:
	inline void pushElements(std::shared_ptr<Elements>&& ele_ptr) {
		ele_ptr->setZindex(int(elementsList.size())), ele_ptr->setId(elementsIdNum++), elementsList.emplace_back(ele_ptr), isNeedUpdate |= true;
	}
	inline int getElementsListSize() { return int(elementsList.size()); }
	inline void setNeedUpdate() { isNeedUpdate |= true; }
private:
	WindowCollection window;
	std::wstring name, title;
	unsigned int id;
	Pos pos;    // be sure change canvas'
	Size size;  // be sure change canvas'
	Canvas canvas;
	bool isNeedUpdate = false, isRefresh = false, isForceRefresh = false, isRun = true;
	WindowMouseInfo mouseInfo;

	// [Elements] held by custom sub class
	unsigned int elementsIdNum = 0;
	std::list<std::shared_ptr<Elements>>elementsList;
	Status elementsPollingUpdate();

	void zindex(unsigned int& ind, unsigned int& ele_zindex);
	void elementsAdjustZindex();

	bool onEvent(Event& e);
	void elementsOnEvent(Event& e);

	void refresh();  // pollingUpdate use only
	
	std::function<void(MouseMoveEvent)> mouseMoveCallback = nullptr;
	std::function<void(MousePrsEvent)> mousePrsCallback = nullptr;
	std::function<void(MouseRlsEvent)> mouseRlsCallback = nullptr;
	std::function<void(WindowResizeEvent)> resizeCallback = nullptr;
	std::function<bool(KeyPrsEvent)> keyPrsCallback = nullptr;
	std::function<void(std::string)> recvCallback = nullptr;
	std::function<Status(void)> pollingCallback = nullptr;

	// run default
	bool _onMouseMove(MouseMoveEvent e);
	bool _onMousePrs(MousePrsEvent e);
	bool _onMouseRls(MouseRlsEvent e);
	bool _onKeyPrs(KeyPrsEvent e);
	void _onResize(ResizeEvent e);
	void _onRecv(RecvEvent e);
};