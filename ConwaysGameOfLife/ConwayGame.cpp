#include "ConwayGame.h"


// Just noting these here
static constexpr long long X_MAX = 2LL << 31;
static constexpr long long X_MIN = -(2LL << 31);
static constexpr long long Y_MAX = 2LL << 31;
static constexpr long long Y_MIN = -(2LL << 31);


static const std::vector<std::pair<int, int>> adjacentDirections = {
	{1, 0}, {0, 1}, {1, 1}, {-1, 0}, {0, -1}, {-1, -1}, {1, -1}, {-1, 1}
};

ConwayGame::ConwayGame()
{
	_latestState.size = 100;
}

void ConwayGame::setAlive(long x, long y, bool alive)
{
	if (_latestState.board[{x, y}] == alive) return;
	_latestState.board[{x, y}] = alive;
	updateSize({ x, y });
	for (const auto& [xDir, yDir] : adjacentDirections) {
		_adjacentCells[{x + xDir, y + yDir}] += (alive) ? 1 : -1;
	}
}

void ConwayGame::step()
{
	_workingCells.clear();
	for (auto& [cellPosition, value] : _adjacentCells) {
		if (value == 3 || (value == 2 && _latestState.board[cellPosition]))
			_workingCells[{cellPosition.x, cellPosition.y}] = true;
	}
	_adjacentCells.clear();
	for (auto& [cellPosition, alive] : _workingCells) {
		updateSize(cellPosition);
		for (const auto& [xDir, yDir] : adjacentDirections) {
			_adjacentCells[{cellPosition.x + xDir, cellPosition.y + yDir}]++;
		}
	}
	
	std::swap(_latestState.board, _workingCells);
}

void ConwayGame::updateSize(const ConwayGame::CellPosition& cellPosition) {
	_latestState.size = std::max(abs(cellPosition.x), _latestState.size);
	_latestState.size = std::max(abs(cellPosition.y), _latestState.size);
}

const ConwayGame::State &ConwayGame::getState() const {
	return _latestState;
}

size_t ConwayGame::CellPositionHash::operator()(const ConwayGame::CellPosition& cell) const
{
	return ((size_t)cell.y << 31) + cell.y;
}
