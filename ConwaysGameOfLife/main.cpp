#include <SFML/Graphics.hpp>
#include <chrono>
#include <thread>
#include "ConwayGameRenderer.h"
int main()
{
    constexpr int WINDOW_WIDTH = 1000;
    constexpr int WINDOW_HEIGHT = 1000;
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Conway's Game of Life");

    FiniteConwayGame game(100, 100);
    FiniteConwayGame::Board testBoard(100, std::vector<FiniteConwayGame::CellState>(100));
    testBoard[49][51] = FiniteConwayGame::Alive;
    testBoard[49][52] = FiniteConwayGame::Alive;
    testBoard[50][50] = FiniteConwayGame::Alive;
    testBoard[50][51] = FiniteConwayGame::Alive;
    testBoard[51][51] = FiniteConwayGame::Alive;
    game.setBoard(testBoard);
    ConwayGameRenderer renderer;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        renderer.render(window, WINDOW_WIDTH, WINDOW_HEIGHT, 0, 0, game.getBoard());
        window.display();
        // std::this_thread::sleep_for(std::chrono::milliseconds(500));
        game.step();
    }

    return 0;
}