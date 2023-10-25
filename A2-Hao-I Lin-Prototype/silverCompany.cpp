//
// Created by Beees on 25/10/2023.
//
#include "silverCompany.h"


 silverCompany::silverCompany(string Name, char idx) : Company(Name, idx) {
     level = 4;
     power = "+ shares";
 }

 string silverCompany::getPower() const {
     return "+ shares";
 }