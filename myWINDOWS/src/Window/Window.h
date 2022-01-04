#pragma once

#include <functional>
#include <vector>

#include "Event.h"
#include "../Graphics/Canvas.h"
#include "../Graphics/Elements.h"
#include "../App/AppCollection.h"

#define WINDOW_API template <typename Tapp, typename Twindow> Window<Tapp, Twindow>

class App;
class Elements;

#define PUSH_ELEMENTS(ele_ptr, ele_entity) ele_ptr.reset(new ele_entity), pushElements(ele_ptr);

class Window
{
public:
	Window(App& app, int _id, std::wstring& _name, Pos& _pos, Size& _size);

	friend class App;
	friend class Elements;

	inline const int& getX() { return pos.x; }
	inline const int& getY() { return pos.y; }
	inline const int& getWidth() { return size.width; }
	inline const int& getHeight() { return size.height; }
	inline const int& getId() { return id; }
	inline const bool& getIsRun() { return isRun; }
	inline Canvas& getCanvas() { return canvas; }
	inline std::vector<std::shared_ptr<Elements>>& getElementsVec() { return elementsVec; }

	inline void setWindowOffset(int offsetX, int offsetY) { pos.x += offsetX, pos.y += offsetY, canvas.setPos(pos); }
	inline void setTitle(std::wstring& str) { title = L"  " + str + L"  ", canvas.getCanvas().replace(3, title.length(), title); }
	inline void setId(unsigned int i) { id = i; }

	inline void setRecieveCallback(std::function<void(std::string)> func) { recieveCallback = func; };
	inline void setMouseMoveCallback(std::function<bool(MouseMoveEvent&)> func) { mouseMoveCallback= func; };
	inline void setMouseClkCallback(std::function<bool(MousePrsEvent&)> func) { mouseClkCallback = func; };
	inline void setKeyPressedCallback(std::function<bool(KeyPrsEvent&)> func) { keyPressedCallback = func; };

	void update();
	inline bool pollingUpdate() { bool b = isNeedUpdate; isNeedUpdate = false; return b; }

protected:
	inline void pushElements(std::shared_ptr<Elements>&& ele_ptr) { ele_ptr->setId(elementsIdNum++), elementsVec.emplace_back(ele_ptr), update(); }
private:
	std::wstring name, title;
	App& app;
	unsigned int id;
	Pos pos;
	Size size;
	Canvas canvas;  // pos, size attr
	
	// element held by custom sub class
	bool isNeedUpdate = false, isRun = true, isFocus = false;
	unsigned int elementsIdNum = 0;
	std::vector<std::shared_ptr<Elements>>elementsVec;

	bool onEvent(Event& e);
	std::function<void(std::string)> recieveCallback = nullptr;
	std::function<bool(MouseMoveEvent&)> mouseMoveCallback = nullptr;
	std::function<bool(MousePrsEvent&)> mouseClkCallback = nullptr;
	std::function<bool(MouseRlsEvent&)> mouseRlsCallback = nullptr;
	std::function<bool(KeyPrsEvent&)> keyPressedCallback = nullptr;

	// run default
	bool windowRecieve(std::string str);
	bool windowMouseMove(MouseMoveEvent& e);
	bool windowMouseClk(MousePrsEvent& e);
	bool windowMouseRls(MouseRlsEvent& e);
	bool windowKeyPressed(KeyPrsEvent& e);
};