//
// Created by Beees on 25/10/2023.
//

#include "bronzeCompany.h"
#include "Player.h"

bronzeCompany::bronzeCompany(string Name, char idx) : Company(Name, idx) {
    level = 3;
    power = "+ money";
}

string bronzeCompany::getPower() const {
    return "+ money";
}
