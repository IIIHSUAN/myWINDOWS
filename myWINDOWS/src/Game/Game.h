#pragma once

#include "../Chess/ChessBoard.h"
#include "../Window/Window.h"

class Game
{
public:
	Game();
	void run();

	void distroy();
private:
	static Game* game;
	ChessBoard chessBoard;
};