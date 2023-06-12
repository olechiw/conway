#include "ConwayGame.h"

static const std::vector<std::pair<int, int>> adjacentDirections = {
	{1, 0}, {0, 1}, {1, 1}, {-1, 0}, {0, -1}, {-1, -1}, {1, -1}, {-1, 1}
};

void ConwayGame::setAlive(int64_t x, int64_t y, bool alive)
{
	if (_latestState.grid[{x, y}] == alive) return;
	_latestState.grid[{x, y}] = alive;
	updateSize({ x, y });
	for (const auto& [xDir, yDir] : adjacentDirections) {
		_adjacentCells[{x + xDir, y + yDir}] += (alive) ? 1 : -1;
	}
}

void ConwayGame::step()
{
	updateWorkingCellsFromAdjacency();

	_adjacentCells.clear();
	for (auto& [cellPosition, alive] : _workingCells) {
		updateSize(cellPosition);
		for (const auto& [xDir, yDir] : adjacentDirections) {
			_adjacentCells[{cellPosition.x + xDir, cellPosition.y + yDir}]++;
		}
	}
	
	std::swap(_latestState.grid, _workingCells);
	_latestState.generations++;
}

void ConwayGame::updateSize(const ConwayGame::CellPosition& cellPosition) {
	_latestState.largestXSeen = std::max(abs(cellPosition.x), _latestState.largestXSeen);
	_latestState.largestYSeen = std::max(abs(cellPosition.y), _latestState.largestYSeen);
}

void ConwayGame::updateWorkingCellsFromAdjacency()
{
	_workingCells.clear();
	for (auto& [cellPosition, value] : _adjacentCells) {
		if (value == 3 || (value == 2 && _latestState.grid[cellPosition]))
			_workingCells.insert({ {cellPosition.x, cellPosition.y}, true });
	}
}

const ConwayGame::State &ConwayGame::getState() const {
	return _latestState;
}

size_t ConwayGame::CellPositionHash::operator()(const ConwayGame::CellPosition& cell) const
{
	return ((size_t)cell.y << 31) + cell.y;
}
