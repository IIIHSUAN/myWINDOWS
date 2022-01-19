#pragma once

#include "ChessCollection.h"



class ChessPiece
{
public:
	ChessPiece() {}
	ChessPiece(ChessType type, ChessColor color, Pos pos) : color(color), pos(pos), lastPos(pos) { setType(type); }

	inline const wchar_t* getSymbol() { return symbol; }
	inline Pos& getPos() { return pos; }
	inline Pos& getLastPos() { return lastPos; }
	inline ChessClass& getChess() { return chess; }
	inline ChessColor& getColor() { return color; }
	inline const bool& getIsAlive() { return isAlive; }

	std::vector<Pos> getMovableVec(int chessIdMap[8][8], ChessPiece chessSet[32], bool isBaseWhite, bool isCastling[2][2], int& lastMoveId, bool isCheckKing = true);

	inline void setPos(Pos _pos) { lastPos = pos; pos = _pos;  }
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
	
	bool isChessmate(int chessIdMap[8][8], ChessPiece chessSet[32], bool isBaseWhite, bool isCastling[2][2], int& lastMoveId, Pos pos = Pos({ -1,-1 }));

	inline bool isSameTeam(int& chessId) { return int(chessId / 16) != int(id / 16); }
private:
	ChessClass chess;
	ChessColor color;
	Pos pos, lastPos;
	int id;
	bool isAlive = true;
	const wchar_t* symbol;
};