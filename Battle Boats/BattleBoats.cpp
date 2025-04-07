#include "BattleBoats.hpp"
#include <iostream>
#include <ctime>

using namespace std;

BattleBoats::BattleBoats() {
    playerGrid = vector<vector<char>>(GRID_SIZE, vector<char>(GRID_SIZE, '_'));
    computerGrid = vector<vector<char>>(GRID_SIZE, vector<char>(GRID_SIZE, '_'));
    guessGrid = vector<vector<char>>(GRID_SIZE, vector<char>(GRID_SIZE, '_'));
    useTwoByOneBoats = false;
    useCustomBoatSymbol = false;
    srand(time(0));
}

void BattleBoats::displayGrids() {
    cout << "\nYou:\t\t\t\tComputer:\n\n   1 2 3 4 5 6 7 8 9 10\t\t   1 2 3 4 5 6 7 8 9 10\n";
    for (int i = 0; i < GRID_SIZE; i++) {
        char rowLabel = 'A' + i;
        cout << rowLabel << "  ";
        for (int j = 0; j < GRID_SIZE; j++)
            cout << playerGrid[i][j] << " ";
        cout << "\t\t" << rowLabel << "  ";
        for (int j = 0; j < GRID_SIZE; j++)
            cout << guessGrid[i][j] << " ";
        cout << endl;
    }
}

Position BattleBoats::getValidPosition(string prompt, const vector<vector<char>>& checkGrid) {
    string input;
    Position pos;
    while (true) {
        cout << prompt;
        cin >> input;
        if (input.length() == 2 && input[0] >= 'A' && input[0] < 'A' + GRID_SIZE && input[1] >= '1' && input[1] <= '9') {
            pos.row = input[0] - 'A';
            pos.col = input[1] - '1';
        }
        else if (input.length() == 3 && input[0] >= 'A' && input[0] < 'A' + GRID_SIZE && input[1] == '1' && input[2] == '0') {
            pos.row = input[0] - 'A';
            pos.col = 9;
        }
        else {
            cout << "Invalid input! Please enter a valid position (e.g., A1, B10).\n";
            continue;
        }

        char cell = checkGrid[pos.row][pos.col];
        if (cell == '_' || cell == boatSymbol || cell == 'B') {
            return pos;
        }
        else {
            cout << "You've already targeted that spot. Try a different one.\n";
        }
    }
}

void BattleBoats::placePlayerBoats() {
    if (useCustomBoatSymbol) {
        cout << "Choose a symbol for your boats: ";
        cin >> boatSymbol;
    }
    else {
        boatSymbol = 'X';
    }

    for (int i = 0; i < NUM_BOATS; i++) {
        while (true) {
            Position start = getValidPosition("Where would you like to place boat " + to_string(i + 1) + "? ", playerGrid);
            Position second = start;
            if (useTwoByOneBoats) {
                char direction;
                cout << "Place horizontally or vertically? (H/V): ";
                cin >> direction;
                if (direction == 'H' || direction == 'h') {
                    second.col = start.col + 1;
                }
                else {
                    second.row = start.row + 1;
                }
                if (second.row >= GRID_SIZE || second.col >= GRID_SIZE) {
                    cout << "That boat goes off the grid. Try again.\n";
                    continue;
                }
                if (playerGrid[second.row][second.col] != '_') {
                    cout << "That boat overlaps with another. Try again.\n";
                    continue;
                }
                playerBoats.push_back(start);
                playerBoats.push_back(second);
                playerGrid[start.row][start.col] = boatSymbol;
                playerGrid[second.row][second.col] = boatSymbol;
            }
            else {
                if (playerGrid[start.row][start.col] != '_') {
                    cout << "That cell is already occupied. Try again.\n";
                    continue;
                }
                playerBoats.push_back(start);
                playerGrid[start.row][start.col] = boatSymbol;
            }
            break;
        }
    }
}

void BattleBoats::placeComputerBoats() {
    for (int i = 0; i < NUM_BOATS; i++) {
        while (true) {
            Position start;
            start.row = rand() % GRID_SIZE;
            start.col = rand() % GRID_SIZE;

            Position second = start;
            bool horizontal = rand() % 2;

            if (useTwoByOneBoats) {
                if (horizontal)
                    second.col++;
                else
                    second.row++;

                if (second.row >= GRID_SIZE || second.col >= GRID_SIZE)
                    continue;

                if (computerGrid[start.row][start.col] != '_' || computerGrid[second.row][second.col] != '_')
                    continue;

                computerBoats.push_back(start);
                computerBoats.push_back(second);
                computerGrid[start.row][start.col] = 'B';
                computerGrid[second.row][second.col] = 'B';
                //uncomment code below to show where the boats are.
                //guessGrid[second.row][second.col] = 'B';
                //guessGrid[start.row][start.col] = 'B';
            }
            else {
                if (computerGrid[start.row][start.col] != '_') continue;
                computerBoats.push_back(start);
                computerGrid[start.row][start.col] = 'B';
                //uncomment code below to show where the boats are.
                //guessGrid[start.row][start.col] = 'B';
            }
            break;
        }
    }
}

bool BattleBoats::playerTurn() {
    Position attackPos = getValidPosition("Pick a spot: ", guessGrid);
    for (auto it = computerBoats.begin(); it != computerBoats.end(); ++it) {
        if (attackPos.row == it->row && attackPos.col == it->col) {
            computerGrid[attackPos.row][attackPos.col] = 'H';
            guessGrid[attackPos.row][attackPos.col] = 'H';
            computerBoats.erase(it);
            cout << "You chose " << char('A' + attackPos.row) << attackPos.col + 1 << ". Hit!\n";
            if (computerBoats.empty()) {
                displayGrids();
                cout << "You sank all boats! You win the game! Great job!\n";
                cout << "========================================\n";
                cout << "|                                      |\n";
                cout << "|                YOU WIN!              |\n";
                cout << "|                                      |\n";
                cout << "========================================\n";
                return true;
            }
            return false;
        }
    }
    computerGrid[attackPos.row][attackPos.col] = 'M';
    guessGrid[attackPos.row][attackPos.col] = 'M';
    cout << "You chose " << char('A' + attackPos.row) << attackPos.col + 1 << ". You missed!\n";
    return false;
}

bool BattleBoats::computerTurn() {
    return computerAI->makeMove();
}

void BattleBoats::playGame() {
    cout << "\nWelcome to Battle Boats!\n";
    char choice;
    char boatsymbolchoice;
    char aiChoice;

    cout << "Choose your opponent:\n";
    cout << "1. Normal (Random)\n2. Row-by-row\n3. Column-by-column\n";
    cin >> aiChoice;
    switch (aiChoice) {
    case '2': aiType = ROW_BY_ROW; break;
    case '3': aiType = COLUMN_BY_COLUMN; break;
    default:  aiType = RANDOM; break;
    }
    computerAI = new ComputerAI(aiType, playerGrid, playerBoats);

    cout << "Do you want to use a custom symbol for your boat? (Default is 'X') (y/n): ";
    cin >> boatsymbolchoice;
    useCustomBoatSymbol = (boatsymbolchoice == 'y' || boatsymbolchoice == 'Y');

    cout << "Would you like to use 2x1 boats instead of 1x1? (y/n): ";
    cin >> choice;
    useTwoByOneBoats = (choice == 'y' || choice == 'Y');

    placePlayerBoats();
    placeComputerBoats();

    cout << "Great! Let's begin!\n";

    while (true) {
        displayGrids();
        if (playerTurn()) break;
        if (computerTurn()) break;
    }
}
