//
// Created by Hao-I Lin on 11/09/2023.
//

#include "Company.h"
#include <iostream>
#include <cstdlib>
#include <sstream>
using namespace std;

// Default constructor for Company
// Default constructor for Company

Company::Company(string Name, int idx) : name(Name), owner("Nobody"), index(idx), acquired(false) {
    switch (index) {
        case 1:
            level = 3;
            power = "+ money";
            break;
        case 2:
            level = 4;
            power = "+ shares";
            break;
        case 3:
            level = 5;
            power = "+ assets";
            break;
        default:
            level = 0;
            power = "";
            break;
    }
    cost = 2 * (5 * level);

    // Ensure shares are at least the cost
    int minShares = cost;
    shares = minShares + rand() % 21;  // Random value between cost and cost+20
    maxShares = shares;
    sharePrice = rand() % (level + 4) + 1; // Random value between 1 and (level + 4)
}


// Destructor
Company::~Company() {}

// Accessors
string Company::getName() const { return name; }
string Company::getOwner() const { return owner; }
string Company::getPower() const { return power; }
char Company::getIndex() const { return index; }
int Company::getCost() const { return cost; }
int Company::getLevel() const { return level; }
int Company::getShares() const { return shares; }
int Company::getMaxShares() const { return maxShares; }
int Company::getSharePrice() const { return sharePrice; }
bool Company::isAcquired() const { return acquired; }


// Mutators
void Company::setName(string name) { this->name = name; }
void Company::setOwner(string owner) { this->owner = owner; }
void Company::setPower(string power) { this->power = power; }
void Company::setIndex(char index) { this->index = index; }
void Company::setCost(int cost) { this->cost = cost; }
void Company::setLevel(int level) { this->level = level; }
void Company::setShares(int shares) { this->shares = shares; }
void Company::setMaxShares(int maxShares) { this->maxShares = maxShares; }
void Company::setSharePrice(int price) { sharePrice = price; }
// Inside the Company class:

// setAcquired method to set the acquired status of the company
// also update shares when the company is acquired
void Company::setAcquired(bool acquired) {
    this->acquired = acquired;
    if (acquired) {
        // Decrease the shares when the company is acquired
        cout << "Shares to remove in set acquired " << this->cost << endl; // Debugging
        addShares(this->cost);
    }
}

void Company::addShares(int sharesToAdd) {
    this->shares += sharesToAdd;
}

void Company::removeShares(int sharesToRemove) {
    // Condition prevent shares to be negative when removing shares
    if (this->shares - sharesToRemove < 0) {
        cout << "shares to remove in remove shares " << sharesToRemove << endl;
        this->shares = 0;
    } else {
        cout << "shares to remove" << sharesToRemove << endl;
        this->shares -= sharesToRemove;
    }
}
