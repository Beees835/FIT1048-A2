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

void Player::addShares(int sharesToAdd) {
    this->totalSharesOwned += sharesToAdd; // Update the player's total shares
}

void Player::buyShares(Company& company, int sharesToBuy) {
    double cost = sharesToBuy * company.getSharePrice();
    if (money >= cost && company.getShares() >= sharesToBuy) {
        money -= cost; // Deduct money

        // Check if the player already owns shares in the company
        bool companyExists = false;
        for (auto& ownedCompany : companyDetails) {
            if (ownedCompany.getName() == company.getName()) {
                companyExists = true;
                break;
            }
        }

        // If the player doesn't own shares in the company, add it to companyDetails
        if (!companyExists) {
            companyDetails.push_back(company);
        }

        totalSharesOwned += sharesToBuy; // Update the player's total shares
        company.removeShares(sharesToBuy); // Remove shares from company
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

vector<Company> Player::getOwnedCompanies() const {
    vector<Company> ownedCompanies;
    for (const auto& company : companyDetails) {
        if (company.getShares() > 0) {
            ownedCompanies.push_back(company);
        }
    }
    return ownedCompanies;
}
