#pragma once
#include <vector>
#include <cassert>
#include <unordered_map>

static constexpr int DEFAULT_CONWAY_SIZE = 5;

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
	struct State {
		Board board;
		long size = DEFAULT_CONWAY_SIZE;
	};

	ConwayGame();
	void setAlive(long x, long y, bool alive = true);
	void step();
	const State& getState() const;
private:
	void updateSize(const ConwayGame::CellPosition& cellPosition);
	void updateWorkingCellsFromAdjacency();
	Board _workingCells;
	State _latestState;
	std::unordered_map<CellPosition, uint8_t, CellPositionHash> _adjacentCells;
};

