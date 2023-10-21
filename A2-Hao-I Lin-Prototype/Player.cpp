//
// Created by Hao-I Lin on 11/09/2023.
//

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include "Player.h"
#include "main.h"

using namespace std;

extern const GameSettings difficultySettings[];
/**
 * Default constructor for Player
 * default difficulty is Easy and default money is 100
 */
Player::Player() : name(""), difficultyLevel(Easy), money(), totalSharesOwned(), totalCompaniesOwned(), powerUsesLeft() {}

Player::Player(string name, Difficulty difficulty) : name(name), difficultyLevel(difficulty) {
    money = 100;
    powerUsesLeft = difficultySettings[static_cast<int>(difficultyLevel)].corporatePowerUses;
    totalSharesOwned = 0;
    totalCompaniesOwned = 0;
}

// Destructor
Player::~Player() {}

// Accessors
string Player::getName() const { return name; }
Difficulty Player::getDifficultyLevel() const { return difficultyLevel; }
double Player::getMoney() const { return money; }
int Player::getTotalSharesOwned() const { return totalSharesOwned; }
int Player::getTotalCompaniesOwned() const { return totalCompaniesOwned; }
int Player::getPowerUsesLeft() const { return powerUsesLeft; }
vector<Company> Player::getCompanyDetails() const { return companyDetails; }

// Mutators
void Player::setName(string name) { this->name = name; }
void Player::setDifficultyLevel(Difficulty difficulty) { this->difficultyLevel = difficulty; }
void Player::setMoney(double money) { this->money = money; }
void Player::setTotalSharesOwned(int shares) { totalSharesOwned = shares; }
void Player::setTotalCompaniesOwned(int companies) { totalCompaniesOwned = companies; }
void Player::setPowerUsesLeft(int power) { powerUsesLeft = power; }
void Player::addCompany(Company company) { companyDetails.push_back(company); }

void Player::addShares(Company& company, int sharesToAdd) {
    company.addShares(company.getShares(), sharesToAdd);
    this->totalSharesOwned += sharesToAdd;
}

void Player::removeShares(Company& company, int sharesToRemove) {
    company.removeShares(company.getShares(), sharesToRemove);
    this->totalSharesOwned -= sharesToRemove;
    if (this->totalSharesOwned < 0) this->totalSharesOwned = 0; // Ensure totalSharesOwned doesn't go below 0
}
