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
    shares = 40 + rand() % 21;  // Random value between 40 and 60
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
void Company::setAcquired(bool acquired) { this->acquired = acquired; }

void Company::addShares(int shares, int sharesToAdd) {
    this->shares = shares + sharesToAdd;
}

void Company::removeShares(int shares, int sharesToRemove) {
    if (shares - sharesToRemove < 0) {
        this->shares = 0;
    } else {
        this->shares = shares - sharesToRemove;
    }
}

BronzeCompany::BronzeCompany(string Name, char index) : Company(Name, index) {}

string BronzeCompany::getPower() const {
    return "+ money";
}

SilverCompany::SilverCompany(string Name, char index) : Company(Name, index) {}

string SilverCompany::getPower() const {
    return "+ shares";
}

GoldCompany::GoldCompany(string Name, char index) : Company(Name, index) {}

string GoldCompany::getPower() const {
    return "+ assets";
}