#pragma once

#include <functional>
#include <list>

#include "Event.h"
#include "Graphics/Canvas.h"
#include "Graphics/Elements.h"
#include "App/AppCollection.h"

class App;
class Elements;

#define PUSH_ELEMENTS(ele_ptr, ele_entity) ele_ptr.reset(new ele_entity), pushElements(ele_ptr)
#define ZINDEX(ele_ptr, index) zIndex((std::shared_ptr<Elements>)ele_ptr, index)
#define FRONT(ind) getElementsListSize() - 1 - ind

class Window
{
public:
	Window(int _id, std::wstring& _name, Pos& _pos, Size& _size, const wchar_t flushChar = L' ');
	
	friend class App;
	friend class Elements;

	inline const int& getX() { return pos.x; }
	inline const int& getY() { return pos.y; }
	inline const Pos& getPos() { return pos; }
	inline const Size& getSize() { return size; }
	inline const int& getId() { return id; }
	inline const bool& getIsRun() { return isRun; }
	inline Canvas& getCanvas() { return canvas; }
	inline std::list<std::shared_ptr<Elements>>& getElementsList() { return elementsList; }

	inline void setWindowOffset(int offsetX, int offsetY) { pos.x += offsetX, pos.y += offsetY, canvas.setPos(pos); }
	inline void setTitle(std::wstring& str) { title = L"  " + str + L"  ", canvas.getCanvas().replace(3, title.length(), title); }
	inline void setId(unsigned int i) { id = i; }

	inline void setRecieveCallback(std::function<void(std::string)> func) { recieveCallback = func; };
	inline void setMouseMoveCallback(std::function<bool(MouseMoveEvent)> func) { mouseMoveCallback = func; };
	inline void setMousePrsCallback(std::function<bool(MousePrsEvent)> func) { mousePrsCallback = func; };
	inline void setKeyPrsCallback(std::function<bool(KeyPrsEvent)> func) { keyPrsCallback = func; };
	inline void setPollingCallback(std::function<void()> func) { pollingCallback = func; };

	bool pollingUpdate();
	
	inline void setIsPollingRefresh(bool b) { isForcePollingRefresh = b; }
protected:
	inline void pushElements(std::shared_ptr<Elements>&& ele_ptr) { 
		ele_ptr->setZindex(int(elementsList.size())), ele_ptr->setId(elementsIdNum++), elementsList.emplace_back(ele_ptr), elementsUpdate();
	}
	int getElementsListSize() { return int(elementsList.size()); }

	void refresh();
private:
	std::wstring name, title;
	unsigned int id;
	Pos pos;
	Size size;
	Canvas canvas;  // pos, size attr
	
	// element held by custom sub class
	bool isNeedUpdate = false, isRun = true, isForcePollingRefresh = false;
	unsigned int elementsIdNum = 0;
	std::list<std::shared_ptr<Elements>>elementsList;
	void elementsUpdate(bool forceUpdate = false);

	void zindex(unsigned int& ind, unsigned int& ele_zindex);
	void ajustZindex();

	bool onEvent(Event& e);
	std::function<void(std::string)> recieveCallback = nullptr;
	std::function<bool(MouseMoveEvent)> mouseMoveCallback = nullptr;
	std::function<bool(MousePrsEvent)> mousePrsCallback = nullptr;
	std::function<bool(MouseRlsEvent)> mouseRlsCallback = nullptr;
	std::function<bool(KeyPrsEvent)> keyPrsCallback = nullptr;
	std::function<void()> pollingCallback = nullptr;

	// run default
	bool windowRecieve(std::string str);
	bool windowMouseMove(MouseMoveEvent e);
	bool windowMousePrs(MousePrsEvent e);
	bool windowMouseRls(MouseRlsEvent e);
	bool windowKeyPrs(KeyPrsEvent e);
};