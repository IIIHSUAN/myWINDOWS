#pragma once

#include "ChessClass.h"

enum ChessId :int {
	none = -16,
	wKing = 0, wQueen, wRook0, wRook1, wBishop0, wBishop1, wKnight0, wKnight1, wPawn0, wPawn1, wPawn2, wPawn3, wPawn4, wPawn5, wPawn6, wPawn7,
	bKing, bQueen, bRook0, bRook1, bBishop0, bBishop1, bKnight0, bKnight1, bPawn0, bPawn1, bPawn2, bPawn3, bPawn4, bPawn5, bPawn6, bPawn7
};

/* implement whole ChessClass Pieces Class ****************************************************/

class King :public ChessClass
{
public:
	King() :ChessClass(ChessType::King, L"♔", L"♚", {
			{-1,-1},{0,-1},{1,-1},
			{-1,0},{0,0},{1,0},
			{-1,1},{0,1},{1,1}
		}) {}
};

class Queen :public ChessClass
{
public:
	Queen() :ChessClass(ChessType::Queen, L"♕", L"♛", {
			// same as king
			{-1,-1},{0,-1},{1,-1},
			{-1,0},{0,0},{1,0},
			{-1,1},{0,1},{1,1},
			{100,100},  // diagonal
			{-100,100},
			{-100,-100},
			{100,-100},
			{0,100}, // orthogonal
			{100,0},
			{0,-100},
			{-100,0}
		}) {}
};

class Rook :public ChessClass
{
public:
	Rook() :ChessClass(ChessType::Rook, L"♖", L"♜", {
			{0,100}, // orthogonal
			{100,0},
			{0,-100},
			{-100,0}
		}) {}
};

class Bishop :public ChessClass
{
public:
	Bishop() :ChessClass(ChessType::Bishop, L"♗", L"♝", {
			{100,100},  // diagonal
			{-100,100},
			{-100,-100},
			{100,-100}
		}) {}
};

class Knight :public ChessClass
{
public:
	Knight() :ChessClass(ChessType::Knight, L"♘", L"♞", {
			{-2,1},{-1,2},{1,2},{2,1},{2,-1},{1,-2},{-1,-2},{-2,-1}
		}) {}
};

class Pawn :public ChessClass
{
public:
	Pawn() :ChessClass(ChessType::Pawn, L"♙", L"♙♟︎", {}) {}  // all pace special
};