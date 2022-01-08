#pragma once

#include "ChessCollection.h"



class ChessPiece
{
public:
	ChessPiece() {}
	ChessPiece(ChessType type, ChessColor color, Pos pos) : color(color), pos(pos) { setType(type); }

	inline const wchar_t* getSymbol() { return symbol; }
	inline Pos& getPos() { return pos; }
	inline ChessClass& getChess() { return chess; }
	inline ChessColor& getColor() { return color; }
	inline const bool& getIsAlive() { return isAlive; }

	std::vector<Pos> getMovableVec(int chessIdMap[8][8], ChessPiece chessSet[32], bool isBaseWhite, std::pair<bool, bool>& isCastling);

	inline void setPos(Pos _pos) { pos = _pos; }
	inline void setAlive(bool b) { isAlive = b; }
	inline void setType(ChessType type) {
		switch (type)
		{
		case ChessType::King:
			chess = King();
			break;
		case ChessType::Queen:
			chess = Queen();
			break;
		case ChessType::Rook:
			chess = Rook();
			break;
		case ChessType::Bishop:
			chess = Bishop();
			break;
		case ChessType::Knight:
			chess = Knight();
			break;
		case ChessType::Pawn:
			chess = Pawn();
			break;
		}

		symbol = chess.getSymbol(color);
	}
	inline void setId(int _id) { id = _id; }

	inline bool move(Pos pace, bool isNegative = false) { if (chess.movable(pace)) { isNegative ? pos -= pace : pos += pace; return true; } return false; }
private:
	ChessClass chess;
	ChessColor color;
	Pos pos;
	int id;
	bool isAlive = true;
	const wchar_t* symbol;
};