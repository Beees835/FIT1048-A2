//
// Created by Hao-I Lin on 11/09/2023.
//

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include "Player.h"
#include "main.h"
#include "Company.h"
#include <map>
#include <set>
#include <fstream>

using namespace std;

extern const GameSettings difficultySettings[];
std::map<std::string, int> sharesOwnedByPlayer; // Key: Company Name, Value: Number of shares owned
std::map<std::string, std::string> acquiredCompanyPowers;
std::set<std::string> acquiredCompanies; // Stores names of companies the player has acquired

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
    acquiredCompanies.clear();  // Initialize the set

    // Read the companies.txt file and populate the companyPowers map
    ifstream file("companies.txt");
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string companyName, power;
        getline(ss, companyName, ';');
        getline(ss, power, ';');
        acquiredCompanyPowers[companyName] = power;
    }
    file.close();
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
        sharesOwnedByPlayer[company.getName()] += sharesToBuy;
        cout << "shares to buy" << sharesToBuy << endl;
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

int Player::getSharesOwnedForCompany(const std::string &companyName) const {
    if (sharesOwnedByPlayer.find(companyName) != sharesOwnedByPlayer.end()) {
        return sharesOwnedByPlayer.at(companyName);
    }
    return 0; // If the player doesn't own shares for this company
}

void Player::sellShares(Company& company, int sharesToSell) {
    // Check if the player owns enough shares to sell
    int ownedShares = getSharesOwnedForCompany(company.getName());
    if (ownedShares >= sharesToSell) {
        int earnings = company.getSharePrice() * sharesToSell; // Calculate the earnings from selling the shares
        money += earnings; // Add earnings to the player's money

        // Update the player's total shares and the shares owned for the specific company
        totalSharesOwned -= sharesToSell;
        sharesOwnedByPlayer[company.getName()] -= sharesToSell;

        // If the player no longer owns shares in the company, remove it from companyDetails
        if (sharesOwnedByPlayer[company.getName()] == 0) {
            for (auto it = companyDetails.begin(); it != companyDetails.end(); ++it) {
                if (it->getName() == company.getName()) {
                    companyDetails.erase(it);
                    break;
                }
            }
        }
        cout << "shares to sell" << sharesToSell << endl;
        company.addShares(sharesToSell); // Add shares back to the company
    } else {
        cout << "Transaction failed. Check if you own enough shares to sell.\n";
    }
}

bool Player::acquireCompany(Company& company) {
    int ownedShares = getSharesOwnedForCompany(company.getName());
    if (ownedShares >= company.getCost()) {
        // Deduct the shares from the player's total for that company
        sharesOwnedByPlayer[company.getName()] -= company.getCost();
        totalSharesOwned -= company.getCost(); // Update the player's total shares

        // Add the company to the player's acquired companies
        acquiredCompanies.insert(company.getName());

        // Increment the total companies owned by the player
        totalCompaniesOwned++;

        // Grant the power associated with the acquired company
        grantPower(company.getName());

        // Store the acquired power
        acquiredCompanyPowers[company.getName()] = company.getPower();

        company.setAcquired(true);
        return true; // Successfully acquired
    }
    return false; // Not enough shares to acquire
}


void Player::grantPower(const std::string &companyName) {
    // Grant the power associated with the acquired company
    string power = acquiredCompanyPowers[companyName];
    if (power == "+ money") {
        // Update player's money
        money += 50;
    } else if (power == "+ shares") {
        // Update player's shares
        totalSharesOwned += 10;
    } else if (power == "+ assets") {
        totalSharesOwned += 10;
        money += 50;
    }
}

const std::map<std::string, std::string> &Player::getAcquiredCompanyPowers() const {
    return acquiredCompanyPowers;
}

bool Player::hasAcquiredCompany(const string &companyName) const {
    return acquiredCompanies.find(companyName) != acquiredCompanies.end();
}

const std::set<std::string> &Player::getAcquiredCompanies() const {
    return acquiredCompanies;
}


