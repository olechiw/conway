#pragma once
#include <vector>
#include <cassert>
#include <unordered_map>

#include <absl/container/flat_hash_map.h>

static constexpr int DEFAULT_CONWAY_SIZE = 5;

class ConwayGame
{
public:
	struct CellPosition {
		int64_t x, y;

		auto operator<=>(const CellPosition& other) const = default;

		template <typename H>
		friend H AbslHashValue(H h, const ConwayGame::CellPosition& cellPosition) {
			return H::combine(std::move(h), cellPosition.x, cellPosition.y);
		}
	};
	struct CellPositionHash {
		size_t operator()(const CellPosition& cell) const;
	};

	using Board = absl::flat_hash_map<CellPosition, bool>;
	struct State {
		Board board;
		int64_t largestXSeen;
		int64_t largestYSeen;
		uint64_t generations = 0;
	};

	ConwayGame() = default;
	~ConwayGame() = default;
	void setAlive(int64_t x, int64_t y, bool alive = true);
	void step();
	const State& getState() const;
private:
	void updateSize(const ConwayGame::CellPosition& cellPosition);
	void updateWorkingCellsFromAdjacency();
	Board _workingCells{};
	State _latestState{};
	absl::flat_hash_map<CellPosition, uint8_t> _adjacentCells {};
};