#include "ConwayGame.h"

static const std::vector<std::pair<int, int>> adjacentDirections = {
	{1, 0}, {0, 1}, {1, 1}, {-1, 0}, {0, -1}, {-1, -1}, {1, -1}, {-1, 1}
};

void ConwayGame::setAlive(ConwayGame::CoordinateType x, ConwayGame::CoordinateType y, bool alive)
{
	if (_latestState.grid.contains({x, y}) == alive) return;
	if (!alive) {
		_latestState.grid.erase({ x, y });
	}
	else {
		_latestState.grid.insert({ x, y });
	}
	updateSize(x, y);
	for (const auto& [xDir, yDir] : adjacentDirections) {
		_adjacentCells[{x + xDir, y + yDir}] += (alive) ? 1 : -1;
	}
}

void ConwayGame::step()
{
	updateWorkingCellsFromAdjacency();

	_adjacentCells.clear();
	for (auto& [x, y] : _workingCells) {
		updateSize(x, y);
		for (const auto& [xDir, yDir] : adjacentDirections) {
			_adjacentCells[{x + xDir, y + yDir}]++;
		}
	}
	
	std::swap(_latestState.grid, _workingCells);
	_latestState.generations++;
}

void ConwayGame::updateSize(ConwayGame::CoordinateType x, ConwayGame::CoordinateType y) {
	_latestState.largestXSeen = std::max(abs(x), _latestState.largestXSeen);
	_latestState.largestYSeen = std::max(abs(y), _latestState.largestYSeen);
}

void ConwayGame::updateWorkingCellsFromAdjacency()
{
	_workingCells.clear();
	for (auto& [cellPosition, value] : _adjacentCells) {
		if (value == 3 || (value == 2 && _latestState.grid.contains(cellPosition)))
			_workingCells.insert(cellPosition);
	}
}

const ConwayGame::State &ConwayGame::getState() const {
	return _latestState;
}
