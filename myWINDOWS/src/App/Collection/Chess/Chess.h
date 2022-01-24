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
	Chess(Pos pos = { 2,1 }) : App(AppCollection::Chess) {
		push_window(window, ChessWindow(0, L"Chess", pos, { MY_WINDOW_WIDTH - 15,MY_WINDOW_HEIGHT - 5 }));

		//App::setPollingUpdate(true);
		
		window->getCanvas().renderWith(board.canvas);
		window->setMousePrsCallback([this](MousePrsEvent& e) {

			ChessWindow& chessWindow = (ChessWindow&)window;

			if (isGame == false)
			{
				window->getCanvas().line(60, 7, std::wstring(L" Game Over "));
				return false;
			}

			e.setPos({ e.getMouseX() - window->getX() , e.getMouseY() - window->getY() });

			if (e.getMouseX() > 0 && e.getMouseX() < window->getSize().width - 1 &&
				e.getMouseY() > 0 && e.getMouseY() < window->getSize().height - 1)
			{
				int opcode = board.onclick(e.getMouseX(), e.getMouseY());
				if (opcode < 0)
					return false;
				else if (opcode == 1)
					window->getCanvas().line(60, 5, std::wstring(L" White Win! ")), isGame = false;
				else if (opcode == 2)
					window->getCanvas().line(60, 5, std::wstring(L" Black Win! ")), isGame = false;

				window->getCanvas().renderWith(board.canvas);
				return true;
			}
			return false;
		});
	}

private:
	std::shared_ptr<Window> window;
	Board board;
	bool isGame = true;
};
