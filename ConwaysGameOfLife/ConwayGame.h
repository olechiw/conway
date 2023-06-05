#pragma once
#include <vector>
#include <cassert>
#include <unordered_map>

class ConwayGame
{
public:
	struct CellPosition {
		long x, y;

		bool operator==(const CellPosition& right) const {
			return x == right.x && y == right.y;
		}
	};
	struct CellPositionHash {
		size_t operator()(const CellPosition& cell) const;
	};
	using Board = std::unordered_map<CellPosition, bool, CellPositionHash>;
	long _size = 100;
private:
	Board _board;
	Board _newBoard;
	std::unordered_map<CellPosition, uint8_t, CellPositionHash> _adjacentCells;
public:
	const Board& getAliveCells() const;
	long size() const;
	void setAlive(long x, long y, bool alive = true);
	void step();
	void updateSize(const ConwayGame::CellPosition& cellPosition);
};

