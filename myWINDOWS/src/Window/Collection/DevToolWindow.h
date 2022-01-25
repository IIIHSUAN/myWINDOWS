#pragma once

#include "../Window.h"

#include "System/System.h"

#define devTool_push(devToolWindow,var) ((DevToolWindow&)*devToolWindow).pushVar(L#var, var);

#define DEVTOOL_PUSH_VEC(vec, Class, var) \
vec.emplace_back((Class&)var,\
new Label(name.c_str(), { Left(2),Top(y) }, *this),\
new Inputbox(std::to_wstring(var).c_str(), { Right(2),Top(y - 1) }, *this, 10, true)\
), pushElements(std::get<1>(vec.back())), pushElements(std::get<2>(vec.back()))

class DevToolWindow : public Window
{
public:
	DevToolWindow(int _id, Pos _pos, Size _size, bool isOpaque = true)
		: isOpaque(isOpaque), Window(_id, std::wstring(L"DevTool"), _pos, _size, IS_OPAQUE(isOpaque), WindowCollection::devTool) {

		/* Window pollingCallback ************************************************************/
		Window::setPollingCallback([this]() {
			for (auto& var : varFloatVec) std::get<2>(var)->setString(std::to_wstring(std::get<0>(var)));
			for (auto& var : varIntVec) std::get<2>(var)->setString(std::to_wstring(std::get<0>(var)));
			for (auto& var : varBoolVec) std::get<2>(var)->setString(std::get<0>(var) ? L"true" : L"false");
		});
	
		/* Window on mouse press ************************************************************/
		Window::setMousePrsCallback([this](MousePrsEvent) {
			toggleTransparent();
			return true;
		});		
	}

	void pushElementsList(std::list<std::shared_ptr<Elements>>& elementsList);
	template<typename T> void pushVar(std::wstring name, T& var);

	inline void toggleTransparent() { isOpaque = !isOpaque; getCanvas().setFlushChar(IS_OPAQUE(isOpaque)); refresh(); }

private:
	bool isOpaque;

	std::vector<std::tuple<float&, std::shared_ptr<Label>, std::shared_ptr<Inputbox>>> varFloatVec;
	std::vector<std::tuple<int&, std::shared_ptr<Label>, std::shared_ptr<Inputbox>>> varIntVec;
	std::vector<std::tuple<bool&, std::shared_ptr<Label>, std::shared_ptr<Inputbox>>> varBoolVec;

	inline unsigned int allVarVecSize() { return unsigned int(varFloatVec.size() + varIntVec.size()); }
	int y = 2;
};

template<typename T>
inline void DevToolWindow::pushVar(std::wstring name, T& var)
{
	if (typeid(float) == typeid(T))
		DEVTOOL_PUSH_VEC(varFloatVec, float, var);
	else if (typeid(int) == typeid(T) || typeid(unsigned int) == typeid(T))
		DEVTOOL_PUSH_VEC(varIntVec, int, var);
	else if(typeid(bool) == typeid(T))
		DEVTOOL_PUSH_VEC(varBoolVec, bool, var);
	
	y += 2;
}