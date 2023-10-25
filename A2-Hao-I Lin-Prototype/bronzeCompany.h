//
// Created by Beees on 25/10/2023.
//

#ifndef FIT1048_BRONZECOMPANY_H
#define FIT1048_BRONZECOMPANY_H
#include "Company.h"
#include "Player.h"

class bronzeCompany : public Company {
public:
    bronzeCompany(string Name, char index);
    string getPower() const override;
};

#endif //FIT1048_BRONZECOMPANY_H
