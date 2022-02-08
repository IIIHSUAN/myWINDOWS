#pragma once

#include "App/App.h"

class Settings;

class SettingsWindow : public Window
{
public:
	SettingsWindow(std::wstring _name, Pos _pos, Size _size) : Window(_name, _pos, _size) {
		
		/* font size ****************************************************************/

		push_elements(lFontSize, Label(L"Ajust Font Size: ", { Left(8),Top(4) },  *this));
		push_elements(iFontSize, Inputbox((std::to_wstring(Output::get().getFontSize()) + L"px").c_str(), { Left(28),Top(3) }, 
			 *this, 17, true));
		iFontSize->onkey([this](KeyPrsEvent& e) {
			if (e.getKey() == KeySet::enter)
			{
				try {
					int fontsize = std::stoi(iFontSize->getString());
					Output::get().setFontSize(fontsize), iFontSize->setString(std::to_wstring(fontsize) + L"px");
					return true;
				} catch (...) { return false; }
			}
			return false;
		});

		/* input async ****************************************************************/

		push_elements(lIsPollingPeriod, Label(L"Polling Period: ", { Left(8),Top(7) },  *this));
		push_elements(bIsPollingPeriod, Button(L" OFF ", { Left(30),Top(6) },  *this, false, { 5,1 }));
		bIsPollingPeriod->onclick([this]() {
			isPollingPeriod = !isPollingPeriod;
			bIsPollingPeriod->setString(isPollingPeriod ? L"  ON  " : L" OFF ");
			System::get().setIsPollingPeriod(isPollingPeriod);
			return true;
		});

		/* set fps ****************************************************************/
		
		push_elements(lFps, Label(L"Set fps: ", { Left(8),Top(10) },  *this));
		push_elements(iFps,
			Inputbox((std::to_wstring(System::get().getFps()) + L"Hz").c_str(),{ Left(28),Top(9) }, *this, 17,true)
		);
		iFps->onkey([this](KeyPrsEvent& e) {
			if (e.getKey() == KeySet::enter)
			{
				try {
					float fps = std::stof(iFps->getString());
					fps = max(fps, 1.0f), iFps->setString(std::to_wstring(fps) + L"Hz");
					System::get().setPollingPeriod(short(1000.0f / fps));
					return true;
				}
				catch (...) { return false; }
			}
			return false;
		});

		/* screen & buffer size ****************************************************************/

		push_elements(lWidth, Label(L"Set Screen Width: ", { Left(8),Top(13) }, *this));
		push_elements(iWidth,
			Inputbox((std::to_wstring(MY_WINDOW_WIDTH) + L"px").c_str(), { Left(28),Top(12) }, *this, 17, true)
		);
		iWidth->onkey([this](KeyPrsEvent& e) {
			if (e.getKey() == KeySet::enter)
			{
				try {
					
					iWidth->setString(std::to_wstring(MY_WINDOW_WIDTH) + L"px");
					return true;
				}
				catch (...) { return false; }
			}
			return false;
		});
		push_elements(lHeight, Label(L"Set Screen Height: ", { Left(8),Top(16) }, *this));
		push_elements(iHeight,
			Inputbox((std::to_wstring(MY_WINDOW_HEIGHT) + L"px").c_str(), { Left(28),Top(15) }, *this, 17, true)
		);
		iHeight->onkey([this](KeyPrsEvent& e) {
			if (e.getKey() == KeySet::enter)
			{
				try {
					
					iHeight->setString(std::to_wstring(MY_WINDOW_HEIGHT) + L"px");
					return true;
				}
				catch (...) { return false; }
			}
			return false;
		});

		/* tips ****************************************************************/

		push_elements(p1, Paragraph(L"[Hot key] set Sensitivity - ctrl & shift            \n          shut down - Esc", 
			{ Left(7),Top(19) }, { {48},{8} }, *this, TextAlign::left, false, { 3,2 })
		);
	}

private:
	std::shared_ptr<Label> lFontSize, lIsPollingPeriod, lFps, lWidth, lHeight;
	std::shared_ptr<Inputbox> iFontSize, iFps, iWidth, iHeight;
	std::shared_ptr<Button> bIsPollingPeriod;
	std::shared_ptr<Paragraph> p1;

	bool isPollingPeriod = false;
};

class Settings :public App
{
public:
	Settings(Pos pos = { 2,1 }, UINT id = 0) : App(AppCollection::Settings, id) {
		push_window(window, SettingsWindow(L"Settings", pos, { 60, 22 }));

		push_window(devToolWindow, DevToolWindow({ MY_WINDOW_WIDTH - 80 + pos.x, 2 + pos.y }, { 40, MY_WINDOW_HEIGHT - 10 }, false));
		devTool_push(devToolWindow, System::get().getFps());
	}

private:
	std::shared_ptr<Window> window, devToolWindow;
};