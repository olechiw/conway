#include <SFML/Graphics.hpp>
#include <chrono>
#include <thread>
#include "ConwayGameRenderer.h"
int main()
{
    constexpr int WINDOW_WIDTH = 1000;
    constexpr int WINDOW_HEIGHT = 1000;
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Conway's Game of Life");

    constexpr int ADDITIONAL_WIDTH = 100;
    constexpr int OFF = ADDITIONAL_WIDTH / 2;
    ConwayGame game;

    game.setAlive(0, 1);
    game.setAlive(0, 2);
    game.setAlive(1, 0);
    game.setAlive(1, 1);
    game.setAlive(2, 1);
    ConwayRenderer renderer;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        renderer.render(window, WINDOW_WIDTH, WINDOW_HEIGHT, 0, 0, game);
        window.display();
        // std::this_thread::sleep_for(std::chrono::milliseconds(500));
        game.step();
    }

    return 0;
}