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
    std::vector<std::pair<long, long>> board = {
            { 0, -1 },
            { 1, -1 },
            { -1, 0},
            { 0, 0 },
            { 0, 1 }
    };

    ConwayGame game;
    for (const auto& [x, y] : board) {
        game.setAlive(x, y);
    }

    constexpr size_t ITER = 3000;
    const auto start = high_resolution_clock::now();
    for (size_t i = 0; i < ITER; ++i) {
        game.step();
        if (i % 1000 == 0)
            cout << i << " ";
    }
    const auto stop = high_resolution_clock::now();

    cout << "Duration: " << duration_cast<microseconds>(stop - start) << endl
        << "Average Step Time: " << duration_cast<microseconds>(stop - start) / ITER << endl;
}