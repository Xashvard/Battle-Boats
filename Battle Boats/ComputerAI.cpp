
#include "ComputerAI.hpp"
#include <cstdlib>
#include <ctime>

ComputerAI::ComputerAI(AIType type, std::vector<std::vector<char>>& grid, std::vector<Position>& boats)
    : aiType(type), playerGrid(grid), playerBoats(boats) {
}

bool ComputerAI::makeMove() {
    Position attackPos;

    // Get next position based on AI type
    if (aiType == RANDOM) {
        do {
            attackPos.row = rand() % playerGrid.size();
            attackPos.col = rand() % playerGrid[0].size();
        } while (playerGrid[attackPos.row][attackPos.col] == 'H' || playerGrid[attackPos.row][attackPos.col] == 'M');
    }
    else if (aiType == ROW_BY_ROW) {
        for (; rowCursor < playerGrid.size(); ++rowCursor) {
            for (; colCursor < playerGrid[rowCursor].size(); ++colCursor) {
                char cell = playerGrid[rowCursor][colCursor];
                if (cell != 'H' && cell != 'M') {
                    attackPos = { rowCursor, colCursor };
                    colCursor++;
                    goto Attack;
                }
            }
            colCursor = 0;
        }
    }
    else if (aiType == COLUMN_BY_COLUMN) {
        for (; colCursor < playerGrid[0].size(); ++colCursor) {
            for (; rowCursor < playerGrid.size(); ++rowCursor) {
                char cell = playerGrid[rowCursor][colCursor];
                if (cell != 'H' && cell != 'M') {
                    attackPos = { rowCursor, colCursor };
                    rowCursor++;
                    goto Attack;
                }
            }
            rowCursor = 0;
        }
    }

Attack:
    std::cout << "The computer chose " << char('A' + attackPos.row) << attackPos.col + 1 << ". ";
    for (auto it = playerBoats.begin(); it != playerBoats.end(); ++it) {
        if (attackPos.row == it->row && attackPos.col == it->col) {
            playerGrid[attackPos.row][attackPos.col] = 'H';
            playerBoats.erase(it);
            std::cout << "Hit!\n";
            if (playerBoats.empty()) {
                std::cout << "========================================\n";
                std::cout << "|                                      |\n";
                std::cout << "|               YOU LOSE!              |\n";
                std::cout << "|                                      |\n";
                std::cout << "========================================\n";
            }
            return playerBoats.empty(); // true if game over
        }
    }

    playerGrid[attackPos.row][attackPos.col] = 'M';
    std::cout << "They missed!\n";
    return false;
}
