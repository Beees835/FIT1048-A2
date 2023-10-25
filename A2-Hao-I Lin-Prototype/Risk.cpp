//
// Created by Hao-I Lin on 11/09/2023.
//

#include "Risk.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <algorithm> // for std::shuffle
#include <random>    // for std::default_random_engine
#include <vector>
using namespace std;
vector<Risk> Risk::allRisks;

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

string Risk::getDetails() {
    stringstream ss;
    ss << "Description: " << description << "\n";
    ss << "Effect: " << effect << "\n";
    ss << "Min Value: " << minValue << "\n";
    ss << "Max Value: " << maxValue << "\n";
    return ss.str();
}

void Risk::loadRisksFromFile(const string& filename) {
    ifstream file(filename);
    string line;

    while (getline(file, line)) {
        stringstream ss(line);
        string description, effect, token;
        int minValue, maxValue;

        getline(ss, description, ';');
        getline(ss, effect, ';');
        getline(ss, token, ';');
        minValue = stoi(token);
        getline(ss, token, ';');
        maxValue = stoi(token);

        allRisks.emplace_back(description, effect, minValue, maxValue);
    }
    // Shuffle the risks after loading
    shuffleRisks();
}

void Risk::shuffleRisks() {
    auto rng = std::default_random_engine{};
    std::shuffle(std::begin(allRisks), std::end(allRisks), rng);
}

Risk Risk::getRandomRisk() {
    int randomIndex = rand() % allRisks.size();
    return allRisks[randomIndex];
}

bool Risk::areRisksLoaded() {
    return !allRisks.empty();
}

