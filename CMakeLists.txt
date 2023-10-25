cmake_minimum_required(VERSION 3.26)
project(fit1048)

set(CMAKE_CXX_STANDARD 14)

include_directories("A2-Hao-I Lin-Prototype")

add_executable(fit1048
        "A2-Hao-I Lin-Prototype/Company.cpp"
        "A2-Hao-I Lin-Prototype/Company.h"
        "A2-Hao-I Lin-Prototype/main.cpp"
        "A2-Hao-I Lin-Prototype/main.h"
        "A2-Hao-I Lin-Prototype/Player.cpp"
        "A2-Hao-I Lin-Prototype/Player.h"
        "A2-Hao-I Lin-Prototype/Risk.cpp"
        "A2-Hao-I Lin-Prototype/Risk.h"
        "A2-Hao-I Lin-Prototype/bronzeCompany.cpp"
        "A2-Hao-I Lin-Prototype/bronzeCompany.h"
        "A2-Hao-I Lin-Prototype/silverCompany.cpp"
        "A2-Hao-I Lin-Prototype/silverCompany.h"
        "A2-Hao-I Lin-Prototype/goldCompany.cpp"
        "A2-Hao-I Lin-Prototype/goldCompany.h"
)
