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

void Player::buyShares(Company& company, int sharesToBuy) {
    double cost = sharesToBuy * company.getSharePrice();
    if (money >= cost && company.getShares() >= sharesToBuy) {
        money -= cost; // Deduct money
        addShares(company, sharesToBuy); // Add shares to player
        company.removeShares(company.getShares(), sharesToBuy); // Remove shares from company
    } else {
        cout << "Transaction failed. Check if you have enough money or if the company has enough shares available.\n";
    }
}

bool Player::canBuyShares(const Company& company, int sharesToBuy) {
    // Calculate the total cost of buying the shares
    double totalCost = company.getSharePrice() * sharesToBuy;

    // Check if the player has enough money
    if (this->money >= totalCost) {
        return true;
    } else {
        return false;
    }
}

// In Player.cpp
vector<Company> Player::getOwnedCompanies() const {
    vector<Company> ownedCompanies;
    for (const auto& company : companyDetails) {
        if (company.getShares() > 0) { // Assuming Company class has a getShares() method
            ownedCompanies.push_back(company);
        }
    }
    return ownedCompanies;
}
