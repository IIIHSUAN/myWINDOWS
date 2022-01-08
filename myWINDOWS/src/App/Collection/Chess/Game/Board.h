#pragma once

#include "Graphics/Canvas.h"
#include "Graphics/Elements.h"

#include "ChessPiece.h"

enum class BaseColor { White, Black };


class Board
{
public:
	Board() {
		canvas = Canvas({ 5, 5 }, { 50,18 }, true);

		resetChess(), refresh();
	}

	friend class ChessWindow;

	void resetChess();

	int onclick(int& x, int& y);  // -1: noUpdate, 0: update, 1: whiteWin, 2: blackWin
	Canvas canvas;
private:
	inline Pos boardPos(Pos pos, bool isRevert = false) { if (isRevert) { pos.x = 7 - pos.x, pos.y = 7 - pos.y; } return pos; }
	bool isBaseWhite = true;
	std::pair<bool, bool> isCastling;
	ChessPiece chessSet[32];
	int chessIdMap[8][8];

	int selectId = ChessId::None;
	Pos selectPos, lastPos = { -1,-1 };
	std::vector<Pos> movableVec;

	void refresh();
	void select(int id);
	int move(Pos pos);
	inline void drawPatch(Canvas& canvas, int x, int y, const wchar_t* c) { canvas.line(x * 2 + 5, y + 2, c, 2); }
	inline void drawChess(Canvas& canvas, int x, int y, const wchar_t* c) { canvas.line(x * 2 + 5, y + 2, c, 1), canvas.line(x * 2 + 6, y + 2, L" ", 1); }

	/***************************************************/

	const wchar_t* frameData = L"\
  ┏━━━━━━━━━━━━━━━━━┓  \
 8┃ ░░  ░░  ░░  ░░  ┃  \
 7┃   ░░  ░░  ░░  ░░┃  \
 6┃ ░░  ░░  ░░  ░░  ┃  \
 5┃   ░░  ░░  ░░  ░░┃  \
 4┃ ░░  ░░  ░░  ░░  ┃  \
 3┃   ░░  ░░  ░░  ░░┃  \
 2┃ ░░  ░░  ░░  ░░  ┃  \
 1┃   ░░  ░░  ░░  ░░┃  \
  ┗━━━━━━━━━━━━━━━━━┛  \
    a b c d e f g h    ";

	CharImage frame = { frameData ,{1,1}, {24,11} };
};