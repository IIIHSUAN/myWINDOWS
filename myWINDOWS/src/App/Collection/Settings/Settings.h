#pragma once

#include "App/App.h"

class Settings;

class SettingsWindow : public Window
{
public:
	SettingsWindow(int _id, std::wstring _name, Pos _pos, Size _size) : Window(_id, _name, _pos, _size) {
		PUSH_ELEMENTS(lFontSize, Label(L"Ajust Font Size: ", { 8,4 }, getSize()));
		PUSH_ELEMENTS(iFontSize, Inputbox(std::to_wstring(Output::get().getFontSize()).c_str(), { 28,3 }, getSize(), 17, true));
		iFontSize->onkey([](KeyPrsEvent& e, std::wstring& str) {
			if (e.getKey() == VK_RETURN)
			{
				try {
					int fontsize = std::stoi(str);
					Output::get().setFontSize(fontsize);
					return true;
				} catch (...) { return false; }
			}
			return false;
		});

		PUSH_ELEMENTS(lInputSync, Label(L"Input Sync: ", { 8,7 }, getSize()));
		PUSH_ELEMENTS(bInputSync, Button(L" OFF ", { 28,6 }, getSize(), true, { 7,1 }));
		bInputSync->onclick([this](Button& b) {
			isInputSync = !isInputSync;
			Input::get().setIsEventUpdate(isInputSync);
			b.setString(isInputSync ? L"  ON  " : L" OFF ");
			return true;
		});

		PUSH_ELEMENTS(lFps, Label(L"Set fps: ", { 8,10 }, getSize()));
		PUSH_ELEMENTS(iFps, Inputbox(std::to_wstring(1.0f / AppHandler::get().getPollingPeriod()).c_str(), 
			{ 28,9 }, getSize(), 17, true)
		);
		iFps->onkey([](KeyPrsEvent& e, std::wstring& str) {
			if (e.getKey() == VK_RETURN)
			{
				try {
					int fps = std::stoi(str);
					AppHandler::get().setPollingPeriod(1.0f / fps);
					return true;
				}
				catch (...) { return false; }
			}
			return false;
		});

		PUSH_ELEMENTS(p1, Paragraph(L"Hot key: set Sensitivity - ctrl & shift\n\nHot key: shut down - Esc",
			{ 7,13 }, { 41,5 }, getSize())
		);
	}

private:
	std::shared_ptr<Label> lFontSize, lInputSync, lFps;
	std::shared_ptr<Inputbox> iFontSize, iFps;
	std::shared_ptr<Button> bInputSync;
	std::shared_ptr<Paragraph> p1;

	bool isInputSync = false;
};

class Settings :public App
{
public:
	Settings(Pos pos = { 2,1 }) : App(AppCollection::Settings) {
		PUSH_WINDOW(window, SettingsWindow(0, L"Settings", pos, { 60, 22 }));
	}

private:
	std::shared_ptr<Window> window;
};
