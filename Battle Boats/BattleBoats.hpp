#pragma once
#ifndef BATTLEBOATS_HPP
#define BATTLEBOATS_HPP

#include <vector>
#include <string>
#include "ComputerAI.hpp"

const int GRID_SIZE = 10;
const int NUM_BOATS = 2;

class BattleBoats {
private:
    std::vector<std::vector<char>> playerGrid;
    std::vector<std::vector<char>> computerGrid;
    std::vector<std::vector<char>> guessGrid;
    std::vector<Position> playerBoats;
    std::vector<Position> computerBoats;
    char boatSymbol;
    bool useTwoByOneBoats;
    bool useCustomBoatSymbol;
    AIType aiType;
    ComputerAI* computerAI;

public:
    void displayGrids();
    Position getValidPosition(std::string prompt, const std::vector<std::vector<char>>& checkGrid);
    void placePlayerBoats();
    void placeComputerBoats();
    bool playerTurn();
    bool computerTurn();
    BattleBoats();
    void playGame();
    void displayPlayerGridOnly();


    bool isUsingTwoByOneBoats() const { return useTwoByOneBoats; }
    bool isUsingCustomBoatSymbol() const { return useCustomBoatSymbol; }
    const std::vector<std::vector<char>>& getComputerGrid() const { return computerGrid; }


    void setupForTesting(bool useTwoByOne) {
        useTwoByOneBoats = useTwoByOne;
        useCustomBoatSymbol = false;
    }


};

#endif
