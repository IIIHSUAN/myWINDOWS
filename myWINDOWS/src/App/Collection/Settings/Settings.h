#pragma once

#include "App/App.h"

class Settings;

class SettingsWindow : public Window
{
public:
	SettingsWindow(int _id, std::wstring _name, Pos _pos, Size _size) : Window(_id, _name, _pos, _size) {
		
		/* font size ****************************************************************/

		PUSH_ELEMENTS(lFontSize, Label(L"Ajust Font Size: ", { Left(8),Top(4) }, getPos(), getSize()));
		PUSH_ELEMENTS(iFontSize, Inputbox((std::to_wstring(Output::get().getFontSize()) + L"px").c_str(), { Left(28),Top(3) }, 
			getPos(), getSize(), 17, true));
		iFontSize->onkey([this](KeyPrsEvent& e) {
			if (e.getKey() == VK_RETURN)
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

		PUSH_ELEMENTS(lInputAsync, Label(L"Input Async: ", { Left(8),Top(7) }, getPos(), getSize()));
		PUSH_ELEMENTS(bInputAsync, Button(L" OFF ", { Left(28),Top(6) }, getPos(), getSize(), true, { 7,1 }));
		bInputAsync->onclick([this]() {
			isInputAsync = !isInputAsync;
			Input::get().setIsEventUpdate(isInputAsync);
			bInputAsync->setString(isInputAsync ? L"  ON  " : L" OFF ");
			return true;
		});

		/* set fps ****************************************************************/

		PUSH_ELEMENTS(lFps, Label(L"Set fps: ", { Left(8),Top(10) }, getPos(), getSize()));
		PUSH_ELEMENTS(iFps, Inputbox((std::to_wstring(1.0f / AppHandler::get().getPollingPeriod()) + L"Hz").c_str(),{ Left(28),Top(9) }, 
			getPos(), getSize(), 17, true)
		);
		iFps->onkey([this](KeyPrsEvent& e) {
			if (e.getKey() == VK_RETURN)
			{
				try {
					float fps = std::stof(iFps->getString());
					fps = max(fps, 0.1f), iFps->setString(std::to_wstring(fps) + L"Hz");
					AppHandler::get().setPollingPeriod(1.0f / fps);
					return true;
				}
				catch (...) { return false; }
			}
			return false;
		});

		/* screen & buffer size ****************************************************************/

		PUSH_ELEMENTS(lwidth, Label(L"Set Screen Width: ", { Left(8),Top(13) }, getPos(), getSize()));
		
		PUSH_ELEMENTS(lheight, Label(L"Set Screen Height: ", { Left(8),Top(16) }, getPos(), getSize()));
		

		/* tips ****************************************************************/

		PUSH_ELEMENTS(p1, Paragraph(L"[Hot key] set Sensitivity - ctrl & shift            \n          shut down - Esc", { Left(7),Top(19) }, 
			{ 48,8 }, getPos(), getSize(), TextAlign::left, false, {3,2})
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
		PUSH_WINDOW(window, SettingsWindow(0, L"Settings", pos, { 60, 22 }));
	}

private:
	std::shared_ptr<Window> window;
};
