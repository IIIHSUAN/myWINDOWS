#pragma once

#include "../App.h"

class Painter :public App
{
public:
	Painter() : App(AppCollection::Painter, windowVec) {
		// App::create(L"Painter", {5,5}, { MY_WINDOW_WIDTH,MY_WINDOW_HEIGHT });
		App::getWindowVec()[0]->getCanvas().setBackground(background);


		std::wstring str = L"10:08:06";
		App::getWindowVec()[0]->getCanvas().line(22, 6, str.c_str(), str.length());

		//Canvas c({ 40,5 }, { 70,10 }, 1, L'/');
		//windowVec[0].getCanvas().renderWithRel(c);
	}

	virtual void run() override {
	}
private:
	std::vector<std::shared_ptr<Window>> windowVec;

	const wchar_t* backgroundData = L"\
***************************************************0\
***** time:          ******************************1\
***************************************************2\
***************************************************3\
***************************************************4\
***************************************************5\
***************************************************6";
	CharImage background = { backgroundData ,{10,5}, {53,7} };

	/******************************************************/

	const wchar_t* bottomData = L"\
++++++++++++++++++\
+++            +++\
++++++++++++++++++";
	CharImage bottom = { backgroundData ,{}, {53,7} };

};
