#pragma once
#include "ConwayGame.h"
#include <cassert>
#include <SFML/Graphics.hpp>

class ConwayRenderer
{
public:
	void render(sf::RenderWindow& window, size_t screenWidth, size_t screenHeight, size_t screenX, size_t screenY, const ConwayGame &game);
};

