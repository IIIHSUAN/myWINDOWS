#pragma once

#include "App/App.h"

class Settings;

class SettingsWindow : public Window
{
public:
	SettingsWindow(int _id, std::wstring _name, Pos _pos, Size _size) : Window(_id, _name, _pos, _size) {
		getCanvas().line(10, 5, L"Ajust font size: ", 17);
		PUSH_ELEMENTS(inputFontSize, Inputbox(std::to_wstring(Output::get().getFontSize()).c_str(), { 28,4 }, 16, true));
		inputFontSize->onkey([](KeyPrsEvent& e, std::wstring& str) {
			if (e.getKey() == VK_RETURN)
			{
				try {
					int fontsize = std::stoi(str);
					Output::get().setFontSize(fontsize);
				}
				catch (...) { return false; }

				return true;
			}
			return false;
		});

		getCanvas().line(10, 10, L"Hot key: set Sensitivity - ctrl & shift", 39);
		getCanvas().line(10, 13, L"Hot key: shut down - Esc", 24);
	}

private:
	std::shared_ptr<Inputbox> inputFontSize;
};

class Settings :public App
{
public:
	Settings(Pos pos = { 2,1 }) : App(AppCollection::Settings, windowVec) {
		CREATE_WINDOW(windowVec, SettingsWindow(0, L"Settings", pos, { 60, 18 }));
	}

private:
	std::vector<std::shared_ptr<Window>> windowVec;
};
