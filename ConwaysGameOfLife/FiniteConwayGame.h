#pragma once
#include <vector>
#include <cassert>

class FiniteConwayGame
{
public:
	using CellState = uint8_t;
	using Board = std::vector<std::vector<CellState>>;
	static constexpr CellState Dead = 0;
	static constexpr CellState Alive = 1;
private:
	static constexpr CellState DeadToAlive = 2;
	static constexpr CellState AliveToDead = 3;
	Board board;
public:
	FiniteConwayGame(const Board& board);
	FiniteConwayGame(size_t width, size_t height);
	const Board& getBoard();
	void setBoard(const Board& board);
	void step();
private:
	uint8_t countNeighbors(size_t x, size_t y);
};

