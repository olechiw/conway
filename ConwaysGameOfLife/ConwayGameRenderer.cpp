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
	const auto rectangleWidth = screenWidth / (gameSize * 2.f);
	const auto rectangleHeight = screenHeight / (gameSize * 2.f);
	sf::RectangleShape aliveCellRectangle(sf::Vector2f(std::max(rectangleWidth - 1, 1.f), std::max(rectangleHeight - 1, 1.f)));
	aliveCellRectangle.setFillColor(sf::Color::Green);
	aliveCellRectangle.setOrigin(rectangleWidth / 2.f, rectangleHeight / 2.f);

	window.clear();
	const auto xOffset = screenWidth / 2;
	const auto yOffset = screenHeight / 2;
	for (const auto& [cellPosition, _] : game.getAliveCells()) {
		aliveCellRectangle.setPosition(cellPosition.x * rectangleHeight + xOffset + screenX, cellPosition.y * rectangleWidth + yOffset + screenY);
		window.draw(aliveCellRectangle);
	}
}
