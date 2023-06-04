#include "FiniteConwayGame.h"


FiniteConwayGame::FiniteConwayGame(const Board& _board)
{
	this->setBoard(_board);
}

FiniteConwayGame::FiniteConwayGame(size_t _width, size_t _height) {
	this->board = std::vector<std::vector<CellState>>(_height, std::vector<CellState>(_width));
}

const FiniteConwayGame::Board& FiniteConwayGame::getBoard()
{
	return this->board;
}

void FiniteConwayGame::setBoard(const Board& _board)
{
	this->board = _board;
}

void FiniteConwayGame::step()
{
	for (size_t x = 0; x < this->board.size(); ++x) {
		for (size_t y = 0; y < this->board[x].size(); ++y) {
			uint8_t aliveNeighbors = countNeighbors(x, y);
			if (board[x][y] == Alive && aliveNeighbors != 2 && aliveNeighbors != 3) {
				board[x][y] = AliveToDead;
			}
			if (board[x][y] == Dead && aliveNeighbors == 3) {
				board[x][y] = DeadToAlive;
			}
		}
	}
	for (size_t x = 0; x < this->board.size(); ++x) {
		for (size_t y = 0; y < this->board[x].size(); ++y) {
			if (board[x][y] == AliveToDead) board[x][y] = Dead;
			if (board[x][y] == DeadToAlive) board[x][y] = Alive;
		}
	}
}

uint8_t FiniteConwayGame::countNeighbors(size_t x, size_t y)
{
	static constexpr CellState EmptyToAlive = 2;
	static constexpr CellState AliveToEmpty = 3;
	assert(!board.empty() && !board.back().empty());

	uint8_t count = 0;
	const auto isAlive = [&](size_t x, size_t y) {
		return board[x][y] == Alive || board[x][y] == AliveToEmpty;
	};
	if (x > 0) {
		count += isAlive(x - 1, y);
	}
	if (y > 0) {
		count += isAlive(x, y - 1);
	}
	if (x > 0 && y > 0) {
		count += isAlive(x - 1, y - 1);
	}
	if (x < board.size() - 1) {
		count += isAlive(x + 1, y);
	}
	if (y < board.back().size() - 1) {
		count += isAlive(x, y + 1);
	}
	if (x < board.size() - 1 && y < board.back().size() - 1) {
		count += isAlive(x + 1, y + 1);
	}
	if (x > 0 && y < board.back().size() - 1) {
		count += isAlive(x - 1, y + 1);
	}
	if (x < board.size() - 1 && y > 0) {
		count += isAlive(x + 1, y - 1);
	}
	return count;
}
