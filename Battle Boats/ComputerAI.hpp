#pragma once
#ifndef COMPUTERAI_HPP
#define COMPUTERAI_HPP

#include <vector>
#include <iostream>

struct Position {
    int row;
    int col;
};

enum AIType {
    RANDOM,
    ROW_BY_ROW,
    COLUMN_BY_COLUMN
};

class ComputerAI {
private:
    AIType aiType;
    std::vector<std::vector<char>>& playerGrid;
    std::vector<Position>& playerBoats;
    int rowCursor = 0;
    int colCursor = 0;

public:
    ComputerAI(AIType type, std::vector<std::vector<char>>& grid, std::vector<Position>& boats);
    bool makeMove();
};

#endif

