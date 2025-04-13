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

void BattleBoats::displayPlayerGridOnly() {
    cout << "\nYour Current Board:\n\n   1 2 3 4 5 6 7 8 9 10\n";
    for (int i = 0; i < GRID_SIZE; i++) {
        char rowLabel = 'A' + i;
        cout << rowLabel << "  ";
        for (int j = 0; j < GRID_SIZE; j++)
            cout << playerGrid[i][j] << " ";
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
            cout << "\nInvalid input! Please enter a valid position (e.g., A1, B10).\n";
            continue;
        }

        char cell = checkGrid[pos.row][pos.col];
        if (cell == '_' || cell == boatSymbol || cell == 'B') {
            return pos;
        }
        else {
            cout << "\nYou've already targeted that spot. Try a different one.\n";
        }
    }
}

void BattleBoats::placePlayerBoats() {
    if (useCustomBoatSymbol) {
        cout << "\nChoose a symbol for your boats: ";
        cin >> boatSymbol;
    }
    else {
        boatSymbol = 'X';
    }

    for (int i = 0; i < NUM_BOATS; i++) {
        while (true) {
            displayPlayerGridOnly();
            Position start = getValidPosition("\nWhere would you like to place boat " + to_string(i + 1) + "? (Use combo of Uppercase letter and a number such as 'A1') ", playerGrid);
            Position second = start;
            if (useTwoByOneBoats) {
                char direction;
                cout << "\nPlace horizontally or vertically? (H/V): ";
                cin >> direction;
                if (direction == 'H' || direction == 'h') {
                    second.col = start.col + 1;
                }
                else {
                    second.row = start.row + 1;
                }
                if (second.row >= GRID_SIZE || second.col >= GRID_SIZE) {
                    cout << "\nThat boat goes off the grid. Try again.\n";
                    continue;
                }
                if (playerGrid[second.row][second.col] != '_') {
                    cout << "\nThat boat overlaps with another. Try again.\n";
                    continue;
                }
                playerBoats.push_back(start);
                playerBoats.push_back(second);
                playerGrid[start.row][start.col] = boatSymbol;
                playerGrid[second.row][second.col] = boatSymbol;
            }
            else {
                if (playerGrid[start.row][start.col] != '_') {
                    cout << "\nThat cell is already occupied. Try again.\n";
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
                //uncomment code below to show where the boats are for 2x1 boats.
                //guessGrid[second.row][second.col] = 'B';
                //guessGrid[start.row][start.col] = 'B';
            }
            else {
                if (computerGrid[start.row][start.col] != '_') continue;
                computerBoats.push_back(start);
                computerGrid[start.row][start.col] = 'B';
                //uncomment code below to show where the boats are gor 1x1 boats.
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
    char soreloserchoice;

    cout << "Choose your opponent: (Type in 1, 2 or 3)\n";
    cout << "\n1. Normal (Random)\n2. Row-by-row\n3. Column-by-column\n\n";
    cin >> aiChoice;
    while (aiChoice != '1' && aiChoice != '2' && aiChoice != '3') {
        cout << "\nInvalid choice! Enter either 1, 2 or 3\n";
        cin.clear();
        cin.ignore();
        cin >> aiChoice;
    }
    switch (aiChoice) {
    case '2': aiType = ROW_BY_ROW; break;
    case '3': aiType = COLUMN_BY_COLUMN; break;
    default:  aiType = RANDOM; break;
    }


    computerAI = new ComputerAI(aiType, playerGrid, playerBoats);

    cout << "\nWould you like to use 2x1 boats instead of 1x1? (y/n): ";
    cin >> choice;
    useTwoByOneBoats = (choice == 'y' || choice == 'Y');

    cout << "\nDo you want to use a custom symbol for your boat? (Default is 'X') (y/n): ";
    cin >> boatsymbolchoice;
    useCustomBoatSymbol = (boatsymbolchoice == 'y' || boatsymbolchoice == 'Y');

    placePlayerBoats();
    placeComputerBoats();

    cout << "\nGreat! Let's begin!\n";

    while (true) {
        displayGrids();
        if (playerTurn()) break;
        if (computerTurn()) break;
    }
}

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void BattleBoats::displayGrids(const vector<vector<char>>& player, const vector<vector<char>>& guesses) {
    cout << "\nPlayer 1 Grid:\t\t\t\tPlayer 2 Grid:\n\n   1 2 3 4 5 6 7 8 9 10\t\t   1 2 3 4 5 6 7 8 9 10\n";
    for (int i = 0; i < GRID_SIZE; i++) {
        char rowLabel = 'A' + i;
        cout << rowLabel << "  ";
        for (int j = 0; j < GRID_SIZE; j++)
            cout << player[i][j] << " ";
        cout << "\t\t" << rowLabel << "  ";
        for (int j = 0; j < GRID_SIZE; j++)
            cout << guesses[i][j] << " ";
        cout << endl;
    }
}

void BattleBoats::displayPlayerGridOnly(const vector<vector<char>>& grid) {
    cout << "\nYour Current Board:\n\n   1 2 3 4 5 6 7 8 9 10\n";
    for (int i = 0; i < GRID_SIZE; i++) {
        char rowLabel = 'A' + i;
        cout << rowLabel << "  ";
        for (int j = 0; j < GRID_SIZE; j++)
            cout << grid[i][j] << " ";
        cout << endl;
    }
}

void BattleBoats::playPvPGame() {
    vector<vector<char>> player2Grid(GRID_SIZE, vector<char>(GRID_SIZE, '_'));
    vector<vector<char>> player2GuessGrid(GRID_SIZE, vector<char>(GRID_SIZE, '_'));
    vector<Position> player2Boats;

    char symbol1 = 'X', symbol2 = 'O';
    bool use2x1;

    cout << "\nPlayer 1, would you like to use 2x1 boats? (y/n): ";
    char c1; cin >> c1;
    use2x1 = (c1 == 'y' || c1 == 'Y');
    useTwoByOneBoats = use2x1;


    cout << "\n=== Player 1: Place Your Boats ===\n";
    cout << "Press Enter when ready...";
    cin.ignore(); cin.get();
    placePlayerBoats();
    clearScreen();

    cout << "\n=== Player 2: Place Your Boats ===\n";
    cout << "Press Enter when ready...";
    cin.get();

    for (int i = 0; i < NUM_BOATS; i++) {
        while (true) {
            displayPlayerGridOnly(player2Grid);
            Position start = getValidPosition("\nWhere would you like to place boat " + to_string(i + 1) + "? ", player2Grid);
            Position second = start;

            if (use2x1) {
                char direction;
                cout << "\nPlace horizontally or vertically? (H/V): ";
                cin >> direction;
                if (direction == 'H' || direction == 'h') {
                    second.col++;
                }
                else {
                    second.row++;
                }
                if (second.row >= GRID_SIZE || second.col >= GRID_SIZE) {
                    cout << "\nThat boat goes off the grid. Try again.\n";
                    continue;
                }
                if (player2Grid[second.row][second.col] != '_') {
                    cout << "\nThat boat overlaps with another. Try again.\n";
                    continue;
                }
                player2Boats.push_back(start);
                player2Boats.push_back(second);
                player2Grid[start.row][start.col] = symbol2;
                player2Grid[second.row][second.col] = symbol2;
            }
            else {
                if (player2Grid[start.row][start.col] != '_') {
                    cout << "\nThat cell is already occupied. Try again.\n";
                    continue;
                }
                player2Boats.push_back(start);
                player2Grid[start.row][start.col] = symbol2;
            }
            break;
        }
    }

    clearScreen();
    cout << "\nLet the battle begin!\n";

    bool isP1Turn = true;
    while (true) {
        clearScreen();
        cout << (isP1Turn ? "\n=== Player 1's Turn ===" : "\n=== Player 2's Turn ===") << endl;

        if (isP1Turn) {
            displayGrids(guessGrid, player2GuessGrid);
            Position attackPos = getValidPosition("Pick a spot: ", player2GuessGrid);
            bool hit = false;
            for (auto it = player2Boats.begin(); it != player2Boats.end(); ++it) {
                if (attackPos.row == it->row && attackPos.col == it->col) {
                    player2Grid[attackPos.row][attackPos.col] = 'H';
                    player2GuessGrid[attackPos.row][attackPos.col] = 'H';
                    player2Boats.erase(it);
                    hit = true;
                    cout << "Hit!\n";
                    break;
                }
            }
            if (!hit) {
                player2GuessGrid[attackPos.row][attackPos.col] = 'M';
                cout << "Miss!\n";
            }
            if (player2Boats.empty()) {
                cout << "\nPlayer 1 wins!\n";
                break;
            }
        }
        else {
            displayGrids(guessGrid, player2GuessGrid);
            Position attackPos = getValidPosition("Pick a spot: ", guessGrid);
            bool hit = false;
            for (auto it = playerBoats.begin(); it != playerBoats.end(); ++it) {
                if (attackPos.row == it->row && attackPos.col == it->col) {
                    playerGrid[attackPos.row][attackPos.col] = 'H';
                    guessGrid[attackPos.row][attackPos.col] = 'H';
                    playerBoats.erase(it);
                    hit = true;
                    cout << "Hit!\n";
                    break;
                }
            }
            if (!hit) {
                guessGrid[attackPos.row][attackPos.col] = 'M';
                cout << "Miss!\n";
            }
            if (playerBoats.empty()) {
                cout << "\nPlayer 2 wins!\n";
                break;
            }
        }

        cout << "Press Enter to end your turn...";
        cin.ignore(); cin.get();
        isP1Turn = !isP1Turn;
    }
}

