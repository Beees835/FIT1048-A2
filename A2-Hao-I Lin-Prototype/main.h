//
// Created by Hao-I Lin on 11/09/2023.
//

#ifndef A2_HAO_I_LIN_PROTOTYPE_MAIN_H
#define A2_HAO_I_LIN_PROTOTYPE_MAIN_H

#include <vector>
#include <string>
#include "Player.h"
#include "Company.h"
#include "Risk.h"

enum Difficulty { Easy, Tricky, Hard };  // Enum for difficulty levels

// Struct for GameSettings
struct GameSettings {
    int gameModeValue;
    int maxCompanies;
    int companiesToWin;
    int minMoneyToWin;
    int maxDaysToPlay;
    int corporatePowerUses;
};



class Player;

void init();
void displayInterface(const GameSettings &currentSettings);
void runGame();
void displayGameIntro(const std::string& filename);
void waitForPlayer();
void clearScreen();
void displayTitle();
void displayPlayerPortfolio(const Player& player);
void displayCompanyDetail();
void buyShares(Player& player);
void sellShares(Player& player);
void acquireCompany(Player& player);
void useCorporatePower(Player& player);
void quitGame();
void takeRisk(Player& player, Difficulty difficulty);
void mergeCompany(Player& player);
void testingMergeCompany(Player& player);
void saveGameState();
bool loadGameState();
void initializeLoadedGameState();
void loadCompanyDetail();

#endif //A2_HAO_I_LIN_PROTOTYPE_MAIN_H