#pragma once

#include <vector>

#include "Graphics/Struct.h"

enum class ChessType { King, Queen, Rook, Bishop, Knight, Pawn };
enum class ChessColor { White, Black };

// ChessClass details the basic info of every chess class, while ChessPiece set pos, canMove, what chess class is

class ChessClass
{
public:
	ChessClass() {}
	ChessClass(ChessType chessType, const wchar_t* whiteSymbol, const wchar_t* blackSymbol, std::vector<Pos>&&moveVec)
		:chessType(chessType), whiteSymbol(whiteSymbol), blackSymbol(blackSymbol), moveVec(moveVec) {
	}

	friend class ChessPiece;

	inline std::vector<Pos>& getPace() { return moveVec; }
	inline ChessType& getType() { return chessType; }
	inline bool movable(Pos pace) {
		for (Pos& move : moveVec)
			if (pace == move)
				return true;
		return false;
	}
private:
	ChessType chessType;
	std::vector<Pos>moveVec;

	const wchar_t*  whiteSymbol;
	const wchar_t*  blackSymbol;
	inline const wchar_t* getSymbol(ChessColor chessColor) { return chessColor == ChessColor::White ? whiteSymbol : blackSymbol; }
};