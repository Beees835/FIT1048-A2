//
// Created by Hao-I Lin on 11/09/2023.
//

#ifndef A2_HAO_I_LIN_PROTOTYPE_COMPANY_H
#define A2_HAO_I_LIN_PROTOTYPE_COMPANY_H

#include <string>
using namespace std;

class Company {
protected:
    string name;
    string owner;
    string power;
    char index;
    int cost;
    int level;
    int shares;
    int maxShares;
    int sharePrice;
    bool acquired;

public:
    Company() : owner("Nobody"), power(""), index(0), cost(0), level(0), shares(0), maxShares(0), sharePrice(0), acquired(false) {}
    Company(string name, int index); // New constructor

    // Destructor
    ~Company();

    // Accessors
    std::string getName() const;
    std::string getOwner() const;

    virtual std::string getPower() const;
    char getIndex() const;
    int getCost() const;
    int getLevel() const;
    int getShares() const;
    int getMaxShares() const;
    int getSharePrice() const;
    bool isAcquired() const;

    // Mutators
    void setName(string name);
    void setOwner(string owner);
    void setPower(string power);
    void setIndex(char index);
    void setCost(int cost);
    void setLevel(int level);
    void setShares(int shares);
    void setMaxShares(int maxShares);
    void setSharePrice(int price);
    void setAcquired(bool acquired);

    // functions to see whether adding and removing shares work
    void removeShares(int sharesToRemove);
    void addShares(int sharesToAdd);
    void increaseShares(int shares);
};

class BronzeCompany : public Company {
public:
    BronzeCompany(string Name, char index);
    string getPower() const override;
};

class SilverCompany : public Company {
public:
    SilverCompany(string Name, char index);
    string getPower() const override;
};

class GoldCompany : public Company {
public:
    GoldCompany(string Name, char index);
    string getPower() const override;
};

#endif //A2_HAO_I_LIN_PROTOTYPE_COMPANY_H
