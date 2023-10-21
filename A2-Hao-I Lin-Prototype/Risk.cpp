//
// Created by Hao-I Lin on 11/09/2023.
//

#include "Risk.h"
#include <iostream>
#include <sstream>
using namespace std;

// Default constructor for Risk
Risk::Risk() : description(""), effect(""), minValue(0), maxValue(0) {}

// Overloaded constructor for Risk
Risk::Risk(string description, string effect, int minValue, int maxValue)
        : description(description), effect(effect), minValue(minValue), maxValue(maxValue) {}

// Accessors
string Risk::getDescription() { return description; }
string Risk::getEffect() { return effect; }
int Risk::getMinValue() { return minValue; }
int Risk::getMaxValue() { return maxValue; }

// Mutators
void Risk::setDescription(string description) { this->description = description; }
void Risk::setEffect(string effect) { this->effect = effect; }
void Risk::setMinValue(int minValue) { this->minValue = minValue; }
void Risk::setMaxValue(int maxValue) { this->maxValue = maxValue; }

// Display details
string Risk::getDetails() {
    stringstream details;
    details << "----- Risk Details -----\n";
    details << "Description: " << description << "\n";
    details << "Effect: " << effect << "\n";
    details << "Min Value: " << minValue << "\n";
    details << "Max Value: " << maxValue << "\n";
    details << "------------------------\n";
    return details.str();
}

// Test function
void Risk::testRisk() {
    cout << "====== Testing Risk class ======\n";

    // Testing default constructor
    Risk defaultRisk;
    cout << "\nTesting Default Constructor Risk Details: \n";
    // Setters testing for default constructor
    defaultRisk.setDescription("Testing Default Risk Description");
    defaultRisk.setEffect("Testing Default Effect");
    defaultRisk.setMinValue(-3);
    defaultRisk.setMaxValue(3);
    // Displaying the details set by the setters
    cout << defaultRisk.getDetails() << endl;

    // Testing overloaded constructor
    Risk marketCrash("Market Crash: Lose Shares ", "shares", 5, -10);
    cout << "\nTesting Overloaded Constructor - Market Crash Risk Details: \n";
    cout << marketCrash.getDetails() << endl;

    Risk advancedRisk("Global Recession: Everyone Loses $ ", "money", -1, -50);
    cout << "\nTesting Overloaded Constructor - Global Recession Details: \n";
    cout << advancedRisk.getDetails() << endl;

    cout << "\n====== End of Risk class tests ======\n";
}

