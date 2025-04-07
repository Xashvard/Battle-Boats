
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
    do {
        BattleBoats game;
        game.playGame();
        cout << "Do you want to play again? (y/n): ";
        cin >> playAgain;
    } while (playAgain == 'y' || playAgain == 'Y');
    cout << "Thanks for playing!\n";
    return 0;
}