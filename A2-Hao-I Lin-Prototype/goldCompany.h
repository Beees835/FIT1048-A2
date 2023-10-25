//
// Created by Beees on 25/10/2023.
//

#ifndef FIT1048_GOLDCOMPANY_H
#define FIT1048_GOLDCOMPANY_H

#include "Company.h"

class goldCompany : public Company {
public:
    goldCompany(string Name, char index);
    string getPower() const override;
};

#endif //FIT1048_GOLDCOMPANY_H
