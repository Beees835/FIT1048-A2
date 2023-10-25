//
// Created by Hao-I Lin on 11/09/2023.
//

#ifndef A2_HAO_I_LIN_PROTOTYPE_PLAYER_H
#define A2_HAO_I_LIN_PROTOTYPE_PLAYER_H

#include <string>
#include <vector>
#include "Company.h"
#include "main.h"
#include <map>
#include <set>
using namespace std;

extern const GameSettings difficultySettings[];
extern std::map<std::string, std::string> acquiredCompanyPowers; // Key: Company Name, Value: Power associated

class Player {
protected:
    string name;
    Difficulty difficultyLevel;  // Changed mode to difficultyLevel of type Difficulty
    double money;
    int totalSharesOwned;
    int totalCompaniesOwned;
    int powerUsesLeft;
    vector<Company> companyDetails;

public:
    // Constructors
    Player();
    Player(string name, Difficulty difficulty);  // Updated the type of mode to Difficulty

    // Destructor
    ~Player();

    // Accessors
    string getName() const;
    Difficulty getDifficultyLevel() const;  // Changed getMode() to getDifficultyLevel() and return type to Difficulty
    double getMoney() const;
    int getTotalSharesOwned() const;
    int getTotalCompaniesOwned() const;
    int getPowerUsesLeft() const;
    vector<Company> getCompanyDetails() const;

    // Mutators
    void setName(string name);
    void setDifficultyLevel(Difficulty difficulty);  // Changed setMode() to setDifficultyLevel() and parameter type to Difficulty
    void setMoney(double money);
    void setTotalSharesOwned(int shares);
    void setTotalCompaniesOwned(int companies);
    void setPowerUsesLeft(int power);
    void addCompany(Company company);
    void addShares(int sharesToAdd);
    void removeShares(Company& company, int sharesToRemove);
    void buyShares(Company& company, int sharesToBuy);
    bool canBuyShares(const Company& company, int sharesToBuy);
    int  getSharesOwnedForCompany(const std::string& companyName) const;
    void sellShares(Company& company, int sharesToSell);
    bool acquireCompany(Company& company);
    void grantPower(const std::string &companyName);

    const std::map<std::string, std::string>& getAcquiredCompanyPowers() const;
    bool hasAcquiredCompany(const std::string& companyName) const;
    const std::set<std::string>& getAcquiredCompanies() const;

    void addSharesForCompany(const string &companyName, int sharesToAdd);

};

#endif //A2_HAO_I_LIN_PROTOTYPE_PLAYER_H