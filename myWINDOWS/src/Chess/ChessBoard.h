#pragma once

#include "Collection/ChessCollection.h"

class ChessBoard
{
public:
	ChessBoard();

	void printChess(const wchar_t* symbol);

	const wchar_t** getBoard();
private:
	std::vector<std::pair<Chess, std::pair<int, int>>> historyMove;
	std::vector<Chess>chessSet;

	void addChess(Chess chess, std::pair<int, int> initPos, bool isSymmetric = true, bool isWhite = true);

	void setChessSymbol(int x,int y,const wchar_t* symbol);
};