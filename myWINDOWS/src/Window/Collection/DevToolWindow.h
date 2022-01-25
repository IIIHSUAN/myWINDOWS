#pragma once

#include "../Window.h"

#include "System/System.h"

#define devTool_push(devToolWindow,var) ((DevToolWindow&)*devToolWindow).pushVar(L#var, var);

class DevToolWindow : public Window
{
public:
	DevToolWindow(int _id, Pos _pos, Size _size, bool isTransparent = false) : Window(_id, std::wstring(L"DevTool"), _pos, _size, isTransparent ? TRANSPARENT_WCHAR : WHITESPACE_WCHAR) {

		/* Window pollingCallback ************************************************************/
		setPollingCallback([this]() {
			updateVarVec();
		});
	}

	inline void updateVarVec() {
		for (auto& var : varFloatVec)
			std::get<2>(var)->setString(std::to_wstring(std::get<0>(var)));
		for (auto& var : varIntVec)
			std::get<2>(var)->setString(std::to_wstring(std::get<0>(var)));
	}

	void pushElementsList(std::list<std::shared_ptr<Elements>>& elementsList);
	template<typename T> void pushVar(std::wstring name, T& var);
private:
	std::vector<std::tuple<float&, std::shared_ptr<Label>, std::shared_ptr<Inputbox>>> varFloatVec;
	std::vector<std::tuple<int&, std::shared_ptr<Label>, std::shared_ptr<Inputbox>>> varIntVec;

	inline unsigned int allVarVecSize() { return unsigned int(varFloatVec.size() + varIntVec.size()); }
	std::shared_ptr<Label> aa;
	int y = 2;
};

template<typename T>
inline void DevToolWindow::pushVar(std::wstring name, T& var)
{
	if(typeid(float)==typeid(T))
		varFloatVec.emplace_back(
			(float&)var,
			new Label(name.c_str(), { Left(2),Top(y) }, *this),
			new Inputbox(std::to_wstring(var).c_str(), { Right(2),Top(y-1) }, *this, 10, true)
		),
		pushElements(std::get<1>(varFloatVec.back())), pushElements(std::get<2>(varFloatVec.back()));
	else if (typeid(int) == typeid(T) || typeid(unsigned int) == typeid(T))
		varIntVec.emplace_back(
			(int&)var,
			new Label(name.c_str(), { Left(2),Top(y) }, *this),
			new Inputbox(std::to_wstring(var).c_str(), { Right(2),Top(y-1) }, *this, 10, true)
		),
		pushElements(std::get<1>(varIntVec.back())), pushElements(std::get<2>(varIntVec.back()));
	
	y += 2;
}