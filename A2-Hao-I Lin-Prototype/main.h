//
// Created by Hao-I Lin on 11/09/2023.
//

#ifndef A2_HAO_I_LIN_PROTOTYPE_MAIN_H
#define A2_HAO_I_LIN_PROTOTYPE_MAIN_H

#include <vector>
#include <string>

// Struct for GameSettings
struct GameSettings {
    int gameModeValue;
    int maxCompanies;
    int companiesToWin;
    int minMoneyToWin;
    int maxDaysToPlay;
    int corporatePowerUses;
};

#include "Player.h"
#include "Company.h"
#include "Risk.h"

class Player;

// Global constants or variables prototypes
extern std::vector<Player> players;
extern std::vector<Risk> risks;
extern const std::string INFO_FILE;

// Function prototypes
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
#endif //A2_HAO_I_LIN_PROTOTYPE_MAIN_H
