#pragma once

#include "App/App.h"
#include "Game/Board.h"

class Chess;

class ChessWindow : public Window
{
public:
	ChessWindow(int _id, std::wstring _name, Pos _pos, Size _size);
};


class Chess :public App
{
public:
	Chess(Pos pos = { 2,1 }) : App(AppCollection::Chess, windowVec) {
		CREATE_WINDOW(windowVec, ChessWindow(0, L"Chess", pos, { MY_WINDOW_WIDTH - 15,MY_WINDOW_HEIGHT - 5 }));

		//App::setPollingUpdate(true);
		
		windowVec[0]->getCanvas().renderWithRel(board.canvas);

		windowVec[0]->setMousePrsCallback([this](MousePrsEvent& e) {

			auto& window = windowVec[0];
			ChessWindow& chessWindow = (ChessWindow&)windowVec[0];

			if (isGame == false)
			{
				window->getCanvas().line(60, 7, std::wstring(L" Game Over "));
				return false;
			}

			e.setPos({ e.getMouseX() - window->getX() , e.getMouseY() - window->getY() });

			if (e.getMouseX() > 0 && e.getMouseX() < window->getWidth() - 1 &&
				e.getMouseY() > 0 && e.getMouseY() < window->getHeight() - 1)
			{
				int opcode = board.onclick(e.getMouseX(), e.getMouseY());
				if (opcode < 0)
					return false;
				else if (opcode == 1)
					window->getCanvas().line(60, 5, std::wstring(L" White Win! ")), isGame = false;
				else if (opcode == 2)
					window->getCanvas().line(60, 5, std::wstring(L" Black Win! ")), isGame = false;

				window->getCanvas().renderWithRel(board.canvas);
				return true;
			}
			return false;
		});
	}

private:
	std::vector<std::shared_ptr<Window>> windowVec;
	Board board;
	bool isGame = true;
};
