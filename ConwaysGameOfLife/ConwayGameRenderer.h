#pragma once
#include "FiniteConwayGame.h"
#include <cassert>
#include <SFML/Graphics.hpp>

class ConwayGameRenderer
{
public:
	void render(sf::RenderWindow& window, size_t screenWidth, size_t screenHeight, size_t screenX, size_t screenY, const FiniteConwayGame::Board &board);
};

