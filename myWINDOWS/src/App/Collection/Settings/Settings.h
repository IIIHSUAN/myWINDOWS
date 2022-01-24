#pragma once

#include "App/App.h"

class Settings;

class SettingsWindow : public Window
{
public:
	SettingsWindow(int _id, std::wstring _name, Pos _pos, Size _size) : Window(_id, _name, _pos, _size) {
		
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

		push_elements(lInputAsync, Label(L"Input Async: ", { Left(8),Top(7) },  *this));
		push_elements(bInputAsync, Button(L" OFF ", { Left(30),Top(6) },  *this, false, { 5,1 }));
		bInputAsync->onclick([this]() {
			isInputAsync = !isInputAsync;
			Input::get().setIsEventUpdate(isInputAsync);
			bInputAsync->setString(isInputAsync ? L"  ON  " : L" OFF ");
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
					System::get().setPollingPeriod(1.0f / fps);
					return true;
				}
				catch (...) { return false; }
			}
			return false;
		});

		/* screen & buffer size ****************************************************************/

		push_elements(lwidth, Label(L"Set Screen Width: ", { Left(8),Top(13) }, *this));
		
		push_elements(lheight, Label(L"Set Screen Height: ", { Left(8),Top(16) }, *this));
		

		/* tips ****************************************************************/

		push_elements(p1, Paragraph(L"[Hot key] set Sensitivity - ctrl & shift            \n          shut down - Esc", { Left(7),Top(19) }, 
			{ {48},{8} }, *this, TextAlign::left, false, { 3,2 })
		);
	}

private:
	std::shared_ptr<Label> lFontSize, lInputAsync, lFps, lwidth, lheight;
	std::shared_ptr<Inputbox> iFontSize, iFps, iwidth, iheight;
	std::shared_ptr<Button> bInputAsync;
	std::shared_ptr<Paragraph> p1;

	bool isInputAsync = false;
};

class Settings :public App
{
public:
	Settings(Pos pos = { 2,1 }) : App(AppCollection::Settings) {
		push_window(window, SettingsWindow(0, L"Settings", pos, { 60, 22 }));
	}

private:
	std::shared_ptr<Window> window;
};