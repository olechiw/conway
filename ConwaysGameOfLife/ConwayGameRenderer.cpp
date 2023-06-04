#include "ConwayGameRenderer.h"

void ConwayGameRenderer::render(
	sf::RenderWindow& window,
	size_t screenWidth,
	size_t screenHeight,
	size_t screenX,
	size_t screenY,
	const FiniteConwayGame::Board &board)
{
	assert(!board.empty() && !board.back().empty());
	const auto rectangleWidth = screenWidth / board.back().size();
	const auto rectangleHeight = screenHeight / board.size();
	assert(rectangleWidth > 1 && rectangleHeight > 1);
	sf::RectangleShape aliveCellRectangle(sf::Vector2f(rectangleWidth - 1, rectangleHeight - 1));
	aliveCellRectangle.setFillColor(sf::Color::Green);

	window.clear();
	for (size_t x = 0; x < board.size(); ++x) {
		for (size_t y = 0; y < board.back().size(); ++y) {
			if (board[x][y] == FiniteConwayGame::Alive) {
				aliveCellRectangle.setPosition(y * rectangleHeight, x * rectangleWidth);
				window.draw(aliveCellRectangle);
			}
		}
	}
}
