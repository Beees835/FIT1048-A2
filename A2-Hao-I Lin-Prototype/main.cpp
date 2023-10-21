/**************************************************************************
 * project name: Risky Business Simulation Game prototype
 * Author: Hao-I Lin
 * Purpose: This program is a prototype of Risky Business Simulation Game.
 **************************************************************************/

#include <iostream>
#include <fstream>
#include <sstream>
#include <limits>
#include <iomanip>
#include "Player.h"
#include "Company.h"
#include "Risk.h"
#include "main.h"

using namespace std;

//////////////////
// Declarations //
//////////////////
std::vector<std::string> companyNames = readCompanyNames("companies.txt");
std::vector<Company> companies;
Difficulty difficulty;
int numPlayers;
int currentMaxDay = 1;
std::vector<Player> players;

const GameSettings difficultySettings[] = {
        {4, 12, 3, 500, 40, 6},  // Easy
        {5, 15, 4, 600, 50, 5},  // Tricky
        {6, 18, 5, 700, 60, 4}   // Hard (-1 for 'all' companies)
};

int main() {
    srand(unsigned(time(nullptr)));  // Seed randoms

    init();                       // Initialize game data
    runGame();                    // Play the game
    waitForPlayer();              // Wait and exit
    return 0;
}

void init() {
    displayGameIntro("RBintro.txt");

    players.clear();  // Clear the vector

    // Get number of players
    do {
        cout << "Enter number of players (1-4): ";
        cin >> numPlayers;
        if (cin.fail() || numPlayers < 1 || numPlayers > 4) {
            cout << "Invalid input! Please enter a number between 1 and 4.\n";
            cin.clear(); // Clear the error state
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignore bad input such as win123ad
        }
    } while (numPlayers < 1 || numPlayers > 4);

    // Get difficulty level
    int choice;
    do {
        cout << "Select difficulty level:\n";
        cout << "1. Easy\n2. Tricky\n3. Hard\n";
        cout << "Enter choice (1-3): ";
        cin >> choice;
        if (cin.fail() || choice < 1 || choice > 3) {
            cout << "Invalid input! Please enter a number between 1 and 3.\n";
            cin.clear(); // Clear the error state
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignore bad input such as win123ad
        }
    } while (choice < 1 || choice > 3);

    difficulty = static_cast<Difficulty>(choice - 1);

    // declare maxCompanies after difficulty is selected and get the value from difficultySettings
    int maxCompanies = difficultySettings[difficulty].maxCompanies;

    // Get player names and set difficulty level for each player using overloaded constructor
    for(int i = 0; i < numPlayers; i++){
        string name;
        cout << "Enter player " << i+1 << "'s name: ";
        cin >> name;
        Player player(name, difficulty);
        players.push_back(player);
    }

    // Initialize companies with default values
    for (int i = 0; i < maxCompanies; i++) {
        char index = i; // Assuming companyNames are in order from A to Z
        Company company = Company(companyNames[i], index);
        companies.push_back(company);
    }
}


void runGame() {
    GameSettings currentSettings = difficultySettings[difficulty];
    displayInterface(currentSettings);

    // Main game loop
    for (int day = 1; day <= currentSettings.maxDaysToPlay; ++day) {
        for (int i = 0; i < numPlayers; ++i) {
            cout << "Player " << (i + 1) << "'s turn:\n";
            // Player's actions go here
        }
        // Update share prices and other end-of-day updates
    }
}

void displayInterface(const GameSettings& currentSettings) {
    displayTitle();
    ostringstream os;
    os << "                 #Companies to win: " << currentSettings.companiesToWin
       << " Min Money: $" << currentSettings.minMoneyToWin << " Day " << currentMaxDay << "                ";
    cout << os.str() << endl;
    cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;

    displayCompanyDetail();

    for (int i =0; i < numPlayers; i++) {
        displayPlayerPortfolio(players[i]);
    }

    cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
    cout << "         [B]uy   [S]ell   [A]cquire   [P]ower   [R]isk   [Q]uit              " << endl;
    cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
    cout << "What will you do now ? ";
    string userInput;
    cin >> userInput; // get user input

}

void displayTitle() {
    cout << endl;
    cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
    cout << "                 Rucky Business :: Share Market Simulation" << endl;
    cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;

}

void displayPlayerPortfolio(const Player& player) {
    cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
    cout << "                    " << player.getName() << "'s Share Portfolio and Assets" << endl;
    cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
    cout << "           Total Money $" << player.getMoney()
         << "   Companies Owned: " << player.getCompanyDetails().size()
         << "    Total Shares: " << player.getTotalSharesOwned() << "\n\n"
         << "          Corporate Power Uses Left: " << player.getPowerUsesLeft() << "\n\n";
    if (player.getTotalSharesOwned() == 0) {
        cout << "                   Your Share Portfolio is empty, " << player.getName() << endl;
    } else {
        // You can add code here to display the shares the player owns
    }
    cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
}

// I prefer using my own file opening function the provided file opening functions don't work
void displayGameIntro(const std::string& filename) {
    static bool introDisplayed = false; // static variable to remember its state across function calls

    if (introDisplayed) {
        return; // If intro has already been displayed, simply return
    }

    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open " << filename << std::endl;
        return;
    }

    std::string line, nextLine;
    if (std::getline(file, line)) {  // Read the first line
        while (std::getline(file, nextLine)) {  // While there's a next line
            std::cout << line << std::endl;  // Display the current line
            line = nextLine;  // Move to the next line
        }
    }
    cout << "\n";
    introDisplayed = true; // Set the flag to true after displaying the intro
}

void displayCompanyDetail() {
    // Displaying the headers in two lines
    cout << setw(30) << left << "Company Name"  // Increased width for "Company Name"
         << setw(15) << "Available"
         << setw(15) << "Current"
         << setw(15) << "Company"
         << setw(10) << "Current" << endl;

    cout << setw(30) << left << ""  // Empty space for "Company Name"
         << setw(15) << "Shares"
         << setw(15) << "Value"
         << setw(15) << "Cost"
         << setw(10) << "Owner" << endl;

    cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;  // Adjusted line length

    for (const auto & company : companies) {
        cout << setw(30) << left << company.getName()  // Adjusted width for company name
             << setw(15) << company.getShares()
             << setw(15) << company.getSharePrice()
             << setw(15) << company.getCost()
             << setw(10) << company.getOwner() << endl;
    }
}

std::vector<std::string> readCompanyNames(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Unable to open " << filename << std::endl;
        return {};  // Return an empty vector if the file can't be opened
    }

    std::vector<std::string> allCompanyNames;
    std::string line;

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string temp;
        if (std::getline(ss, temp, ';') && std::getline(ss, temp, ';')) {  // Ensure both parts exist
            temp.erase(0, temp.find_first_not_of(" \t\n\r"));  // remove leading whitespace
            temp.erase(temp.find_last_not_of(" \t\n\r") + 1);  // remove trailing whitespace
            allCompanyNames.push_back(temp);
        } else {
            std::cerr << "Warning: Unexpected format in line: " << line << std::endl;
        }
    }
    return allCompanyNames;
}


//Company randomCompanyInitializer(string name, char index) {
//    // Default owner is set to "None" since no player owns the company initially
//    string defaultOwner = "None";
//
//    // Assuming there are 5 types of powers, we randomly select one for the company
//    string powers[] = {"Power1", "Power2", "Power3", "Power4", "Power5"};
//    string power = powers[rand() % 5];
//
//    // Randomly generate other attributes for the company based on the new constraints
//    int cost = rand() % 31 + 30;               // Random cost between 30 and 60
//    int level = rand() % 5 + 1;                // Random level between 1 and 5
//    int shares = rand() % 41 + 40;             // Random shares between 40 and 80
//    int maxShares = 100;                       // Assuming max shares for all companies are 100
//    int sharePrice = rand() % 4 + 3;           // Random share price between 3 and 6
//    bool acquired = false;                     // Initially, no company is acquired
//
//    // Return a Company object initialized with the generated values
//    return Company(name, defaultOwner, power, cost, level, shares, maxShares, sharePrice, index, acquired);
//}


////////////////////////////
// Helper functions below //
////////////////////////////

void waitForPlayer() {
    // adjust new line and indents as required
    // Windows
    cout << "\n\t ";
    system("pause");
    clearScreen();
    // Mac & Linux
    //system("read -n 1 -s -r -p \"Press any key to continue\"");
}

void clearScreen() {
    // Windows
    system("cls");
    // Mac & Linux
    //system("clear");
}
