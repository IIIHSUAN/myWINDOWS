#include "Board.h"

#include "Window/Window.h"

void Board::resetChess()
{
	chessSet[wKing] = ChessPiece(ChessType::King, ChessColor::White, boardPos({ 3,0 }, isBaseWhite));
	chessSet[wQueen] = ChessPiece(ChessType::Queen, ChessColor::White, boardPos({ 4,0 }, isBaseWhite));
	chessSet[wRook0] = ChessPiece(ChessType::Rook, ChessColor::White, boardPos({ 0,0 }, isBaseWhite));
	chessSet[wRook1] = ChessPiece(ChessType::Rook, ChessColor::White, boardPos({ 7,0 }, isBaseWhite));
	chessSet[wBishop0] = ChessPiece(ChessType::Bishop, ChessColor::White, boardPos({ 2,0 }, isBaseWhite));
	chessSet[wBishop1] = ChessPiece(ChessType::Bishop, ChessColor::White, boardPos({ 5,0 }, isBaseWhite));
	chessSet[wKnight0] = ChessPiece(ChessType::Knight, ChessColor::White, boardPos({ 1,0 }, isBaseWhite));
	chessSet[wKnight1] = ChessPiece(ChessType::Knight, ChessColor::White, boardPos({ 6,0 }, isBaseWhite));
	chessSet[wPawn0] = ChessPiece(ChessType::Pawn, ChessColor::White, boardPos({ 0,1 }, isBaseWhite));
	chessSet[wPawn1] = ChessPiece(ChessType::Pawn, ChessColor::White, boardPos({ 1,1 }, isBaseWhite));
	chessSet[wPawn2] = ChessPiece(ChessType::Pawn, ChessColor::White, boardPos({ 2,1 }, isBaseWhite));
	chessSet[wPawn3] = ChessPiece(ChessType::Pawn, ChessColor::White, boardPos({ 3,1 }, isBaseWhite));
	chessSet[wPawn4] = ChessPiece(ChessType::Pawn, ChessColor::White, boardPos({ 4,1 }, isBaseWhite));
	chessSet[wPawn5] = ChessPiece(ChessType::Pawn, ChessColor::White, boardPos({ 5,1 }, isBaseWhite));
	chessSet[wPawn6] = ChessPiece(ChessType::Pawn, ChessColor::White, boardPos({ 6,1 }, isBaseWhite));
	chessSet[wPawn7] = ChessPiece(ChessType::Pawn, ChessColor::White, boardPos({ 7,1 }, isBaseWhite));

	chessSet[bKing] = ChessPiece(ChessType::King, ChessColor::Black, boardPos({ 3,0 }, !isBaseWhite));
	chessSet[bQueen] = ChessPiece(ChessType::Queen, ChessColor::Black, boardPos({ 4,0 }, !isBaseWhite));
	chessSet[bRook0] = ChessPiece(ChessType::Rook, ChessColor::Black, boardPos({ 0,0 }, !isBaseWhite));
	chessSet[bRook1] = ChessPiece(ChessType::Rook, ChessColor::Black, boardPos({ 7,0 }, !isBaseWhite));
	chessSet[bBishop0] = ChessPiece(ChessType::Bishop, ChessColor::Black, boardPos({ 2,0 }, !isBaseWhite));
	chessSet[bBishop1] = ChessPiece(ChessType::Bishop, ChessColor::Black, boardPos({ 5,0 }, !isBaseWhite));
	chessSet[bKnight0] = ChessPiece(ChessType::Knight, ChessColor::Black, boardPos({ 1,0 }, !isBaseWhite));
	chessSet[bKnight1] = ChessPiece(ChessType::Knight, ChessColor::Black, boardPos({ 6,0 }, !isBaseWhite));
	chessSet[bPawn0] = ChessPiece(ChessType::Pawn, ChessColor::Black, boardPos({ 0,1 }, !isBaseWhite));
	chessSet[bPawn1] = ChessPiece(ChessType::Pawn, ChessColor::Black, boardPos({ 1,1 }, !isBaseWhite));
	chessSet[bPawn2] = ChessPiece(ChessType::Pawn, ChessColor::Black, boardPos({ 2,1 }, !isBaseWhite));
	chessSet[bPawn3] = ChessPiece(ChessType::Pawn, ChessColor::Black, boardPos({ 3,1 }, !isBaseWhite));
	chessSet[bPawn4] = ChessPiece(ChessType::Pawn, ChessColor::Black, boardPos({ 4,1 }, !isBaseWhite));
	chessSet[bPawn5] = ChessPiece(ChessType::Pawn, ChessColor::Black, boardPos({ 5,1 }, !isBaseWhite));
	chessSet[bPawn6] = ChessPiece(ChessType::Pawn, ChessColor::Black, boardPos({ 6,1 }, !isBaseWhite));
	chessSet[bPawn7] = ChessPiece(ChessType::Pawn, ChessColor::Black, boardPos({ 7,1 }, !isBaseWhite));

	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 8; j++)
			chessIdMap[i][j] = ChessId::None;
	for (int id = 0; id < 32; id++)
	{
		Pos& pos = chessSet[id].getPos();
		chessIdMap[pos.y][pos.x] = id;

		chessSet[id].setId(id);
	}

	isCastling = { false,false };
}

int Board::onclick(int& x, int& y)
{
	Pos pos = Pos({ int((x - canvas.getPos().x - 5) / 2),(y - canvas.getPos().y - 2) });
	if (pos.x >= 8 || pos.x < 0 || pos.y >= 8 || pos.y < 0)
		return -1;

	if (selectId >= 0)
		for (auto&movement : movableVec)
			if (movement == pos && (movement.x != 0 || movement.y != 0))
				return move(pos);

	if (chessIdMap[pos.y][pos.x] >= 0)
	{
		select(chessIdMap[pos.y][pos.x]);
		return 0;
	}

	return -1;
}

void Board::select(int id)  // legal move decision
{
	if (selectId != id)
	{
		selectId = id;
		selectPos = chessSet[id].getPos();
		movableVec = chessSet[id].getMovableVec(chessIdMap, chessSet, isBaseWhite, isCastling);

		refresh();
		for (Pos& movement : movableVec)
			if (movement.x != 0 || movement.y != 0)
				drawPatch(canvas, movement.x, movement.y, L"❉ ");
	}
	else
		selectId = ChessId::None, refresh();
}

int Board::move(Pos pos)
{
	int winner = 0;
	movableVec.clear();

	if (chessIdMap[pos.y][pos.x] >= 0)
	{
		if (chessIdMap[pos.y][pos.x] == ChessId::bKing)
			winner = 1;
		else if (chessIdMap[pos.y][pos.x] == ChessId::wKing)
			winner = 2;

		chessSet[chessIdMap[pos.y][pos.x]].setAlive(false);
	}

	lastPos = chessSet[selectId].getPos();
	chessIdMap[lastPos.y][lastPos.x] = ChessId::None;


	chessIdMap[pos.y][pos.x] = selectId;
	chessSet[selectId].setPos(pos);

	// pawn special
	if (pos.y == 7 || pos.y == 0)
		chessSet[selectId].setType(ChessType::Queen);

	selectId = ChessId::None;
	refresh();
	return winner;
}

void Board::refresh()
{
	canvas.setBackground(frame);
	for (auto& chess : chessSet)
	{
		Pos& p = chess.getPos();

		if(chess.getIsAlive())
			drawChess(canvas, p.x, p.y, chess.getSymbol());
	}
}