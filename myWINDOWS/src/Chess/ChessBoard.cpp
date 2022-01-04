#include "ChessBoard.h"

#include <windows.h>

ChessBoard::ChessBoard()
{
	addChess(Rook(), { 0, 0 });
	addChess(Knight(), { 1, 0 });
	addChess(Bishop(), { 2, 0 });
	addChess(King(), { 3, 0 }, false, true);
	addChess(King(), { 3, 7 }, false, false);
	addChess(Queen(), { 4, 0 }, false, true);
	addChess(Queen(), { 4, 7 }, false, false);

	for(int i=0;i<4;i++)
		addChess(Pawn(), { i, 1 });
}

void ChessBoard::printChess(const wchar_t* symbol)
{
	DWORD n;
	HANDLE cons = GetStdHandle(STD_OUTPUT_HANDLE);
	WriteConsoleW(cons, symbol, wcslen(symbol), &n, NULL);
}

const wchar_t** ChessBoard::getBoard()
{
	static const char rowName[]  = "12345678";
	printf("\n     a b c d e f g h\n    -----------------\n");
	for (int i = 0; i < 8; i++)
	{
		printf("  %c |", rowName[i]);
		for (int j = 0; j < 8; j++)
			printChess(chessSet[3].getSymbol()),printf("|");
		printf("\n    -----------------\n");
	}
	return nullptr;
}

void ChessBoard::addChess(Chess chess, std::pair<int, int> initPos, bool isSymmetric, bool isWhite)
{
	chess.setColor(isWhite);
	chess.setPos(initPos.first, initPos.second);
	if (isSymmetric)
		chess.setPos(7 - initPos.first, initPos.second);
	chessSet.push_back(chess);

	if (isSymmetric)
	{
		chess.setColor(!isWhite);
		chess.setPos(initPos.first, 7 - initPos.second);
		chess.setPos(7 - initPos.first, 7 - initPos.second);
		chessSet.push_back(chess);
	}
}

void ChessBoard::setChessSymbol(int x, int y, const wchar_t * symbol)
{
	//sizeof(symbol) / sizeof(wchar_t)
	std::wstring s = symbol+ std::wstring(L"AAAAAAAAAAAA");
	DWORD n;
	HANDLE cons = GetStdHandle(STD_OUTPUT_HANDLE);
	WriteConsoleW(cons, s.c_str(), wcslen(s.c_str()), &n, NULL);
	return;
}
