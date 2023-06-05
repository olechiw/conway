#include <SFML/Graphics.hpp>
#include <chrono>
#include <thread>
#include "ConwayGameRenderer.h"

// Our Project/SLN targets the Windows subsystem
// This is only possible because we link to sfml-main.lib which defines main as WINMAIN when on windows
int main()
{
    constexpr int WINDOW_WIDTH = 2560;
    constexpr int WINDOW_HEIGHT = 1440;
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Conway's Game of Life");

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
        renderer.render(window, 1000, 1000, WINDOW_WIDTH / 2 - 500, WINDOW_HEIGHT / 2 - 500, game);
        window.display();
        // std::this_thread::sleep_for(std::chrono::milliseconds(500));
        game.step();
    }

    return 0;
}