
#include <iostream>
#include "ComputerAI.hpp"
#include "BattleBoats.hpp"
using namespace std;

int main() {
    cout << "========================================\n";
    cout << "|                                      |\n";
    cout << "|              BATTLE BOATS            |\n";
    cout << "|                                      |\n";
    cout << "========================================\n";
    char playAgain;
    cout << "\nChoose mode:\n1. Player vs Computer\n2. Player vs Player (local)\n(Type in either 1 or 2)\n\n";
    char mode;
    cin >> mode;
    while (mode != '1' && mode != '2') {
        cout << "\nInvalid choice! Enter either 1 or 2\n";
        cin.clear();
        cin.ignore();
        cin >> mode;
    }
    do {
        BattleBoats game;
        if (mode == '1') game.playGame();
        else if (mode == '2') game.playPvPGame();
        cout << "Do you want to play again? (y/n): ";
        cin >> playAgain;
    } while (playAgain == 'y' || playAgain == 'Y');
    cout << "Thanks for playing!\n";
    return 0;
}