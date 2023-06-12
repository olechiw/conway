#include <chrono>
#include <vector>
#include <iostream>

#include "ConwayGame.h"

using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::chrono::nanoseconds;
using std::chrono::milliseconds;
using std::chrono::microseconds;
using std::cout;
using std::endl;

int main() {
    std::vector<std::pair<std::string, std::vector<std::pair<int64_t, int64_t>>>> grids =
    { 
        { "R-Grid", {
            { 0, -1 },
            { 1, -1 },
            { -1, 0},
            { 0, 0 },
            { 0, 1 }} 
        }
    };

    constexpr size_t ITER = 30000;

    for (const auto& [name, grid] : grids) {
        ConwayGame game;
        for (const auto& [x, y] : grid)
            game.setAlive(x, y);

        const auto start = high_resolution_clock::now();
        for (size_t i = 0; i < ITER; ++i) {
            game.step();
        }
        const auto stop = high_resolution_clock::now();

        cout << endl
            << "----" << name << "----" << endl
            << "Duration: " << duration_cast<microseconds>(stop - start) << endl
            << "Average Step Time: " << duration_cast<microseconds>(stop - start) / ITER << endl;
    }
}