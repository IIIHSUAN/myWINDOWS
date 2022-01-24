#include "ChessPiece.h"

std::vector<Pos> ChessPiece::getMovableVec(int chessIdMap[8][8], ChessPiece chessSet[32], bool isBaseWhite, bool isCastling[2][2], int& lastMoveId, bool isCheckKing)
{
	std::vector<Pos> result;
	static int orient, drawX, drawY, orientX, orientY;
	orient = (isBaseWhite ^ (color == ChessColor::White)) ? 1 : -1;

	// pawn special
	if (chess.getType() == ChessType::Pawn)
	{
		// hostile front-left
		drawX = pos.x - 1, drawY = pos.y + orient;
		if (drawX >= 0 && chessIdMap[drawY][drawX] != ChessId::none && isSameTeam(chessIdMap[drawY][drawX]))
			result.emplace_back(Pos({ drawX,drawY }));
		// hostile front-right
		drawX = pos.x + 1, drawY = pos.y + orient;
		if (drawX <= 7 && chessIdMap[drawY][drawX] != ChessId::none &&  isSameTeam(chessIdMap[drawY][drawX]))
			result.emplace_back(Pos({ drawX,drawY }));
		
		// hostile left && hostile pawn first two stride
		drawX = pos.x - 1, drawY = pos.y;
		if (drawY == (orient > 0 ? 4 : 3) &&
			chessSet[lastMoveId].chess.getType() == ChessType::Pawn && chessSet[lastMoveId].color != color &&
			chessSet[lastMoveId].lastPos == Pos({ drawX,drawY + orient * 2 }))
			result.emplace_back(Pos({ drawX,drawY + orient }));;
		// hostile right && hostile pawn first two stride
		drawX = pos.x + 1, drawY = pos.y;
		if (drawY == (orient > 0 ? 4 : 3) &&
			chessSet[lastMoveId].chess.getType() == ChessType::Pawn && chessSet[lastMoveId].color != color &&
			chessSet[lastMoveId].lastPos == Pos({ drawX,drawY + orient * 2 }))
			result.emplace_back(Pos({ drawX,drawY + orient }));;

		// straight
		if (chessIdMap[pos.y + orient][pos.x] == ChessId::none)
		{
			result.emplace_back(Pos({ pos.x,pos.y + orient }));  // x1

			if (pos.y == ((orient > 0) ? 1 : 6) && chessIdMap[pos.y + orient * 2][pos.x] == ChessId::none)  // start, x2
				result.emplace_back(Pos({ pos.x,pos.y + orient * 2 }));
		}
	}

	// castling, not check checkmate
	if (id==ChessId::wKing)
	{
		if (isCastling[0][0] && chessIdMap[pos.y][pos.x + 1] == ChessId::none && chessIdMap[pos.y][pos.x + 2] == ChessId::none)
			result.emplace_back(Pos({ pos.x + 2,pos.y }));
		if (isCastling[0][1] && chessIdMap[pos.y][pos.x - 1] == ChessId::none && chessIdMap[pos.y][pos.x - 2] == ChessId::none && chessIdMap[pos.y][pos.x - 3] == ChessId::none)
			result.emplace_back(Pos({ pos.x - 2,pos.y }));
	}
	else if (id == ChessId::bKing)
	{
		if (isCastling[1][0] && chessIdMap[pos.y][pos.x + 1] == ChessId::none && chessIdMap[pos.y][pos.x + 2] == ChessId::none && chessIdMap[pos.y][pos.x + 3] == ChessId::none)
			result.emplace_back(Pos({ pos.x + 2,pos.y }));
		if (isCastling[1][1] && chessIdMap[pos.y][pos.x - 1] == ChessId::none && chessIdMap[pos.y][pos.x - 2] == ChessId::none)
			result.emplace_back(Pos({ pos.x - 2,pos.y }));
	}

	/* regular rule *****************************************************/

	for (auto& pace : chess.getPace())
		if (pace.x == 100 || pace.x == -100 || pace.y == 100 || pace.y == -100)  // vec
		{
			orientX = pace.x / 100, orientY = pace.y / 100;

			for (int i = 1; i < 8; i++)
			{
				drawX = pos.x + orientX * i, drawY = pos.y + orientY * i;

				if (drawX >= 0 && drawX < 8 && drawY >= 0 && drawY < 8)
				{
					if (chessIdMap[drawY][drawX] == ChessId::none)
						result.emplace_back(Pos({ drawX,drawY }));
					else if (isSameTeam(chessIdMap[drawY][drawX]))  // id not in same color
					{
						result.emplace_back(Pos({ drawX,drawY }));
						break;
					}
					else
						break;
				}
			}
		}
		else
		{
			drawX = pos.x + pace.x;
			drawY = pos.y + pace.y *orient;
			if (drawX >= 0 && drawX < 8 && drawY >= 0 && drawY < 8 &&
				isSameTeam(chessIdMap[drawY][drawX]))  // id not in same color
				result.emplace_back(Pos({ drawX,drawY }));
		}

	if (isCheckKing && chess.getType() == ChessType::King)
		for (int i = (orient == 1 ? ChessId::wKing : ChessId::bKing) + 1; i <= (orient == 1 ? wPawn7 : bPawn7); i++)
		{
			auto& enemyVec = chessSet[i].getMovableVec(chessIdMap, chessSet, isBaseWhite, isCastling, lastMoveId, false);

			for (auto& enemyMove : enemyVec)
				for (auto& rlt : result)
					if (enemyMove == rlt)
					{
						rlt.x = -1, rlt.y = -2;
						break;
					}
		}
	
	return result;
}