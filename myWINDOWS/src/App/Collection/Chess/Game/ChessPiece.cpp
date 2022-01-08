#include "ChessPiece.h"

std::vector<Pos> ChessPiece::getMovableVec(int chessIdMap[8][8], ChessPiece chessSet[32], bool isBaseWhite, std::pair<bool, bool>& isCastling)
{
	std::vector<Pos> result;
	bool isWhite = (color == ChessColor::White);
	int orient = (isBaseWhite ^ isWhite) ? 1 : -1;
	static int drawX, drawY, orientX, orientY;

	// pawn special
	if (chess.getType() == ChessType::Pawn)
	{
		drawX = pos.x - 1, drawY = pos.y + orient;

		if (chessIdMap[drawY][drawX] >= 0 && int(chessIdMap[drawY][drawX] / 16) != int(id / 16))  // hostile left
			result.emplace_back(Pos({ drawX,drawY }));
		drawX = pos.x + 1, drawY = pos.y + orient;
		if (chessIdMap[drawY][drawX] >= 0 && int(chessIdMap[drawY][drawX] / 16) != int(id / 16))  // hostile right
			result.emplace_back(Pos({ drawX,drawY }));

		// straight
		if (chessIdMap[pos.y + orient][pos.x] < 0)
		{
			result.emplace_back(Pos({ pos.x,pos.y + orient }));  // x1

			if (pos.y == ((orient > 0) ? 1 : 6) && chessIdMap[pos.y + orient * 2][pos.x] < 0)  // start, x2
				result.emplace_back(Pos({ pos.x,pos.y + orient * 2 }));
		}

		return result;
	}

	// king & rook special
	if (chess.getType() == ChessType::King || chess.getType() == ChessType::Rook)
	{
		if (!isCastling.first&&isBaseWhite)
			;
		else if (!isCastling.second && !isBaseWhite)
			;
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
					if (chessIdMap[drawY][drawX] < 0)
						result.emplace_back(Pos({ drawX,drawY }));
					else if (int(chessIdMap[drawY][drawX] / 16) != int(id / 16))  // id not in same color
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
				int(chessIdMap[drawY][drawX] / 16) != int(id / 16))  // id not in same color
				result.emplace_back(Pos({ drawX,drawY }));
		}


	return result;
}
