#pragma once

#include <string>
#include <vector>

class Chess
{
public:
	Chess() {}
	Chess(const wchar_t* _name, const wchar_t* _whi_symbol, const wchar_t* _blk_symbol, std::vector<std::pair<int, int>> _moveFeasibleVec)
		:name(_name), whi_symbol(_whi_symbol), blk_symbol(_blk_symbol), moveFeasibleVec(_moveFeasibleVec) {}
	virtual ~Chess() {}

	void setColor(bool _isWhite) { isWhite = _isWhite; }

	std::pair<int, int> getPos() { return pos; }
	void setPos(int x,int y);

	const std::vector<std::pair<int, int>>& getMoveVec() const { return moveFeasibleVec; }

	inline const wchar_t* getSymbol() { return isWhite ? whi_symbol : blk_symbol; }

	virtual bool isMoveFeasible(std::pair<int, int> move);  // & special rule
private:
	const wchar_t* name;
	const wchar_t* whi_symbol;
	const wchar_t* blk_symbol;
	bool isWhite = true;
	std::pair<int, int> pos;
	std::vector<std::pair<int, int>> moveFeasibleVec;
	bool isAlive = true;
};