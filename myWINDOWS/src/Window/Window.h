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
	bool isPrsFrame = false, isPrsCorner = false;
	Pos last;
	inline void setLast(Pos&& pos) { last = pos; }
};

enum class WindowCollection { unknown, default, devTool };
class Window
{
public:
	Window(int _id, std::wstring& _name, Pos& _pos, Size& _size, const wchar_t flushChar = WHITESPACE_WCHAR, WindowCollection window = WindowCollection::default);

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

	inline void setTitle(std::wstring& str) { canvas.line(3, 0, L"  " + str + L"  "); }
	inline void setId(unsigned int i) { id = i; }

	inline void setRecieveCallback	 (std::function<void(std::string)> func)		{ recvCallback = func; };
	inline void setMouseMoveCallback (std::function<void(MouseMoveEvent)> func)		{ mouseMoveCallback = func; };
	inline void setMousePrsCallback	 (std::function<void(MousePrsEvent)> func)		{ mousePrsCallback = func; };
	inline void setKeyPrsCallback	 (std::function<bool(KeyPrsEvent)> func)		{ keyPrsCallback = func; };
	inline void setResizeCallback	 (std::function<void(WindowResizeEvent)> func)	{ resizeCallback = func; };
	inline void setPollingCallback	 (std::function<void()> func)					{ pollingCallback = func; };

	bool pollingUpdate();

protected:
	inline void pushElements(std::shared_ptr<Elements>&& ele_ptr) {
		ele_ptr->setZindex(int(elementsList.size())), ele_ptr->setId(elementsIdNum++), elementsList.emplace_back(ele_ptr), elementsUpdate();
	}
	int getElementsListSize() { return int(elementsList.size()); }

	void refresh();
private:
	WindowCollection window;
	std::wstring name, title;
	unsigned int id;
	Pos pos;    // be sure change canvas'
	Size size;  // be sure change canvas'
	Canvas canvas;
	WindowMouseInfo mouseInfo;

	// [Elements] held by custom sub class
	bool isNeedUpdate = false, isRun = true;
	unsigned int elementsIdNum = 0;
	std::list<std::shared_ptr<Elements>>elementsList;
	void elementsUpdate();

	void zindex(unsigned int& ind, unsigned int& ele_zindex);
	void adjustZindex();

	bool onEvent(Event& e);
	bool elementsOnEvent(Event& e);
	
	

	std::function<void(MouseMoveEvent)> mouseMoveCallback = nullptr;
	std::function<void(MousePrsEvent)> mousePrsCallback = nullptr;
	std::function<void(MouseRlsEvent)> mouseRlsCallback = nullptr;
	std::function<void(WindowResizeEvent)> resizeCallback = nullptr;
	std::function<bool(KeyPrsEvent)> keyPrsCallback = nullptr;
	std::function<void(std::string)> recvCallback = nullptr;
	std::function<void()> pollingCallback = nullptr;

	// run default
	bool _onMouseMove(MouseMoveEvent e);
	bool _onMousePrs(MousePrsEvent e);
	bool _onMouseRls(MouseRlsEvent e);
	bool _onKeyPrs(KeyPrsEvent e);
	void _onRecv(std::string str);
};