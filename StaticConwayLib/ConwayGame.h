#pragma once
#include <vector>
#include <cassert>
#include <unordered_map>

#include <absl/container/flat_hash_map.h>
#include <absl/container/flat_hash_set.h>

static constexpr int DEFAULT_CONWAY_SIZE = 5;

class ConwayGame
{
public:
	using CoordinateType = int64_t;
	struct CellPosition {
		CoordinateType x, y;

		auto operator<=>(const CellPosition& other) const = default;

		template <typename H>
		friend H AbslHashValue(H h, const ConwayGame::CellPosition& cellPosition) {
			return H::combine(std::move(h), cellPosition.x, cellPosition.y);
		}
	};

	using Grid = absl::flat_hash_set<CellPosition>;

	struct State {
		Grid grid;
		CoordinateType largestXSeen = DEFAULT_CONWAY_SIZE;
		CoordinateType largestYSeen = DEFAULT_CONWAY_SIZE;
		uint64_t generations = 0;
	};

	ConwayGame() = default;
	~ConwayGame() = default;
	void setAlive(CoordinateType x, CoordinateType y, bool alive = true);
	void step();
	const State& getState() const;
private:
	void updateSize(CoordinateType x, CoordinateType y);
	void updateWorkingCellsFromAdjacency();
	Grid _workingCells{};
	State _latestState{};
	absl::flat_hash_map<CellPosition, uint8_t> _adjacentCells {};
};