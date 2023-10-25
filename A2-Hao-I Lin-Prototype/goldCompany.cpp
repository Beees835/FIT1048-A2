//
// Created by Beees on 25/10/2023.
//

#include "goldCompany.h"

goldCompany::goldCompany(string Name, char idx) : Company(Name, idx) {
    level = 5;
    power = "+ assets";
}

string goldCompany::getPower() const {
    return "+ assets";
}