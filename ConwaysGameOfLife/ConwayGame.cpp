#include "ConwayGame.h"


// Just noting these here
static constexpr long long X_MAX = 2LL << 31;
static constexpr long long X_MIN = -(2LL << 31);
static constexpr long long Y_MAX = 2LL << 31;
static constexpr long long Y_MIN = -(2LL << 31);


static const std::vector<std::pair<int, int>> adjacentDirections = {
	{1, 0}, {0, 1}, {1, 1}, {-1, 0}, {0, -1}, {-1, -1}, {1, -1}, {-1, 1}
};

void ConwayGame::setAlive(long x, long y, bool alive)
{
	if (_board[{x, y}] == alive) return;
	_board[{x, y}] = alive;
	updateSize({x, y});
	for (const auto &[xDir, yDir] : adjacentDirections) {
		_adjacentCells[{x + xDir, y + yDir}] += (alive) ? 1 : -1;
	}
}

void ConwayGame::step()
{
	_newBoard.clear();
	for (auto& [cellPosition, value] : _adjacentCells) {
		if (value == 3 || (value == 2 && _board[cellPosition]))
			_newBoard[{cellPosition.x, cellPosition.y}] = true;
	}
	_adjacentCells.clear();
	for (auto& [cellPosition, alive] : _newBoard) {
		updateSize(cellPosition);
		for (const auto& [xDir, yDir] : adjacentDirections) {
			_adjacentCells[{cellPosition.x + xDir, cellPosition.y + yDir}]++;
		}
	}
	std::swap(_newBoard, _board);
}

void ConwayGame::updateSize(const ConwayGame::CellPosition &cellPosition) {
	if (abs(cellPosition.y) > _size)
		_size = abs(cellPosition.y);
	if (abs(cellPosition.x) > _size)
		_size = abs(cellPosition.x);
}

long ConwayGame::size() const {
	return _size;
}

const ConwayGame::Board& ConwayGame::getAliveCells() const {
	return _board;
}

size_t ConwayGame::CellPositionHash::operator()(const ConwayGame::CellPosition& cell) const
{
	return ((size_t)cell.y << 31) + cell.y;
}
