#pragma once

#include "Chess.h"

/* implement whole Chess Pieces Class ****************************************************/

class King :public Chess
{
public:
	King() :Chess(L"King", L"♔", L"♚", {
			{-1,-1},{0,-1},{1,-1},
			{-1,0},{0,0},{1,0},
			{-1,1},{0,1},{1,1}
		}) {}
};

class Queen :public Chess
{
public:
	Queen() :Chess(L"Queen", L"♕", L"♛", {
			{100,100},  // diagonal
			{0,100}, // orthogonal
			// same as king
			{-1,-1},{0,-1},{1,-1},
			{-1,0},{0,0},{1,0},
			{-1,1},{0,1},{1,1}
		}) {}
};

class Rook :public Chess
{
public:
	Rook() :Chess(L"Rook", L"♖", L"♜", {
			{0,100} // orthogonal
		}) {}
};

class Bishop :public Chess
{
public:
	Bishop() :Chess(L"Bishop", L"♗", L"♝", {
			{100,100}  // diagonal
		}) {}
};

class Knight :public Chess
{
public:
	Knight() :Chess(L"Knight", L"♘", L"♞", {
			{-2,1},{-1,2},{1,2},{2,1},{2,-1},{1,-2},{-1,-2},{-2,-1}
		}) {}
};

class Pawn :public Chess
{
public:
	Pawn() :Chess(L"Pawn", L"♙", L"♟︎", {
			{0,1},{0,2}
		}) {}
};