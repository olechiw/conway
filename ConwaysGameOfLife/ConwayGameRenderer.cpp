#include "ConwayGameRenderer.h"

void ConwayRenderer::render(
	sf::RenderWindow& window,
	size_t screenWidth,
	size_t screenHeight,
	size_t screenX,
	size_t screenY,
	const ConwayGame& game)
{
	const long gameSize = game.size();
	const auto rectangleWidth = screenWidth / (gameSize * 2);
	const auto rectangleHeight = screenHeight / (gameSize * 2);
	assert(rectangleWidth > 1 && rectangleHeight > 1);
	sf::RectangleShape aliveCellRectangle(sf::Vector2f(rectangleWidth - 1, rectangleHeight - 1));
	aliveCellRectangle.setFillColor(sf::Color::Green);

	window.clear();
	const auto xOffset = screenWidth / 2;
	const auto yOffset = screenHeight / 2;
	for (const auto& [cellPosition, _] : game.getAliveCells()) {
		aliveCellRectangle.setPosition(cellPosition.y * rectangleHeight + yOffset, cellPosition.x * rectangleWidth + xOffset);
		window.draw(aliveCellRectangle);
	}
}
