﻿#define TEST_MODE

#include <cassert>
#include "BattleBoats.hpp"
#include "ComputerAI.hpp"

using namespace std;

#include <cassert>
#include <iostream>
using namespace std;

void runTests() {
    cout << "Running tests...\n";

    Position p1 = { 3, 5 };
    Position p2 = { 3, 5 };
    Position p3 = { 4, 2 };
    assert(p1.row == p2.row && p1.col == p2.col);
    assert(!(p1.row == p3.row && p1.col == p3.col));

    vector<vector<char>> dummyGrid(GRID_SIZE, vector<char>(GRID_SIZE, '_'));
    vector<Position> dummyBoats = { {0, 0}, {1, 1} };
    ComputerAI ai(RANDOM, dummyGrid, dummyBoats);
    assert(ai.makeMove() == false);

    BattleBoats game;
    game.setupForTesting(true); 
    game.placeComputerBoats();
    const auto& grid = game.getComputerGrid();

    int boatCount = 0;
    for (int i = 0; i < GRID_SIZE; ++i)
        for (int j = 0; j < GRID_SIZE; ++j)
            if (grid[i][j] == 'B')
                boatCount++;
    assert(boatCount == NUM_BOATS * 2); 
    assert(game.isUsingTwoByOneBoats() == true);
    assert(game.isUsingCustomBoatSymbol() == false);

    BattleBoats overlapTest;
    overlapTest.setupForTesting(false);
    overlapTest.placeComputerBoats(); // 1x1 boats
    const auto& overlapGrid = overlapTest.getComputerGrid();
    int singles = 0;
    for (int i = 0; i < GRID_SIZE; ++i)
        for (int j = 0; j < GRID_SIZE; ++j)
            if (overlapGrid[i][j] == 'B')
                singles++;
    assert(singles == NUM_BOATS);

    cout << "All tests passed successfully.\n";
}

int main() {
#ifdef TEST_MODE
    runTests();
    return 0;
#endif
}