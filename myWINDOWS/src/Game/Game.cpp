#include <stdio.h>
#include <iostream>

#include "Game.h"


Game* Game::game = nullptr;

Game::Game()
{
	if (game != nullptr)
	{
		printf("ASSERT: only can create one game");
		return;
	}
	else
		game = this;
}

void Game::run()
{
	char input[128];
	std::string parser;
	while (std::cin.getline(input, 128))
	{
		system("cls");
		parser = input;
		if (parser.find("set ") == 0)
			;
		else
			chessBoard.getBoard();
	}
}

void Game::distroy()
{
	if (game != nullptr)
		delete game;
}
