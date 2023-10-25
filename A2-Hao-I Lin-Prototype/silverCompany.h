//
// Created by Beees on 25/10/2023.
//

#ifndef FIT1048_SILVERCOMPANY_H
#define FIT1048_SILVERCOMPANY_H

#include "Company.h"

class silverCompany : public Company{
public:
    silverCompany(string Name, char index);
    string getPower() const override;
};


#endif //FIT1048_SILVERCOMPANY_H
