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
#include <set>
#include "Player.h"
#include "Company.h"
#include "Risk.h"
#include "main.h"

using namespace std;

//////////////////
// Declarations //
//////////////////
std::vector<Company*> companies;  // Using pointers to store company objects
Difficulty difficulty;
int currentMaxDay = 1;
Player player;

const GameSettings difficultySettings[] = {
        {4, 12, 3, 500, 40, 6},  // Easy
        {5, 15, 4, 600, 50, 5},  // Tricky
        {6, 18, 5, 700, 60, 4}   // Hard
};

int main() {
    srand(unsigned(time(nullptr)));  // Seed randoms
    init();                       // Initialize game data
    runGame();                    // Play the game
    waitForPlayer();              // Wait and exit

    // Cleanup: delete dynamically allocated Company objects
    for (auto companyPtr : companies) {
        delete companyPtr;
    }

    return 0;
}

void init() {
    displayGameIntro("RBintro.txt");

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

    // Get player name
    string name;
    cout << "Enter your name: ";
    cin >> name;
    player = Player(name, difficulty);

    // Open the companies.txt file for reading
    ifstream file("companies.txt");
    if (file.is_open()) {
        string line;
        int companyCount = 0; // Counter for the number of companies read

        // Set the number of companies based on difficulty level
        int maxCompanies;
        switch (difficulty) {
            case Difficulty::Easy:   maxCompanies = 12; break;
            case Difficulty::Tricky: maxCompanies = 15; break;
            case Difficulty::Hard:   maxCompanies = 18; break;
            default:                 maxCompanies = 12; break; // Default to EASY mode
        }

        // Read the file line by line and create Company objects using Polymorphism
        while (getline(file, line) && companyCount < maxCompanies) {
            stringstream ss(line);
            int index;
            string name, power;
            char delimiter;

            ss >> index >> delimiter;
            getline(ss, name, ';');
            getline(ss, power, ';');

            Company* newCompany;
            if (power == "+ money") {
                newCompany = new BronzeCompany(name, index);
            } else if (power == "+ shares") {
                newCompany = new SilverCompany(name, index);
            } else if (power == "+ assets") {
                newCompany = new GoldCompany(name, index);
            } else {
                newCompany = new Company(name, index);
            }

            companies.push_back(newCompany);
            companyCount++; // Increment the counter
        }
        file.close();
    } else {
        cout << "Error: Unable to open companies.txt file." << endl;
    }
}

void runGame() {
    GameSettings currentSettings = difficultySettings[difficulty];
    displayInterface(currentSettings);

    // Main game loop
    for (int day = 1; day <= currentSettings.maxDaysToPlay; ++day) {
        char userAction;
        cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
        cout << "         [B]uy   [S]ell   [A]cquire   [P]ower   [R]isk   [Q]uit              " << endl;
        cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
        cout << "What will you do now ? " << endl;
        cin >> userAction;
        switch (userAction) {
            case 'B' | 'b':
                buyShares(player);
                displayInterface(currentSettings);  // Return to main interface after finished action
                break;
            case 'S' | 's':
                sellShares(player);
                displayInterface(currentSettings);  // Return to main interface after finished action
                break;
            case 'A' | 'a':
                acquireCompany(player);
                displayInterface(currentSettings);  // Return to main interface after finished action
                break;
            case 'P' | 'p':
                useCorporatePower(player);
                displayInterface(currentSettings);
                break;
            case 'R' | 'r':
                // Take a risk
                break;
            case 'Q' | 'q':
                quitGame();
                break;
            default:
                cout << "Invalid input!\n";
                break;
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
    displayPlayerPortfolio(player);
}

void displayTitle() {
    cout << endl;
    cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
    cout << "                 Rucky Business :: Share Market Simulation" << endl;
    cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
}

void displayPlayerPortfolio(const Player& player) {
    cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
    cout <<  "                        " << player.getName() << "'s Share Portfolio and Assets" << endl;
    cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
    cout << "Total Money $" << setw(10) << player.getMoney()
         << "Companies Owned: " << setw(5) << player.getAcquiredCompanies().size()
         << "Total Shares: " << setw(5) << player.getTotalSharesOwned() << "\n\n"
         << "Corporate Power Uses Left: " << setw(5) << player.getPowerUsesLeft() << "\n\n";

    if (player.getTotalSharesOwned() == 0 && player.getAcquiredCompanies().empty()) {
        cout << "Your Share Portfolio is empty, " << player.getName() << endl;
    } else {
        cout << left << setw(30) << "Company Names" << setw(20) << "Shares" << setw(20) << "Power" << endl;
        const auto& acquiredPowers = player.getAcquiredCompanyPowers();

        // Display acquired companies and companies where shares are owned
        for (const auto& company : companies) {
            string companyName = company->getName();
            int sharesOwned = player.getSharesOwnedForCompany(companyName);

            if (sharesOwned > 0 || player.hasAcquiredCompany(companyName)) {
                string power = "No";
                if (acquiredPowers.find(companyName) != acquiredPowers.end()) {
                    power = acquiredPowers.at(companyName);
                }
                cout << left << setw(30) << companyName << setw(20) << sharesOwned << setw(20) << power << endl;
            }
        }
    }
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
    cout << setw(30) << left << "Company Name"
         << setw(15) << "Available"
         << setw(15) << "Current"
         << setw(15) << "Company"
         << setw(10) << "Current" << endl;

    cout << setw(30) << left << ""
         << setw(15) << "Shares"
         << setw(15) << "Value"
         << setw(15) << "Cost"
         << setw(10) << "Owner" << endl;

    cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;

    for (const auto& companyPtr : companies) {
        cout << setw(30) << left << companyPtr->getName()
             << setw(15) << companyPtr->getShares()
             << setw(15) << companyPtr->getSharePrice()
             << setw(15) << companyPtr->getCost()
             << setw(10) << companyPtr->getOwner() << endl;
    }
}

void buyShares(Player& player) {
    bool stillBuying = true;
    while (stillBuying) {
        // Display available companies
        cout << "Available companies:\n";
        for (const auto & company : companies) {
            if (company->getShares() == 0) {
                cout << company->getName() << " - No shares available!" << "\n";
            } else {
                cout << company->getName() << " - Available shares: " << company->getShares()
                     << " - Share price: $" << company->getSharePrice() << "\n";
            }
        }

        // Get user choice
        char choiceChar;
        cout << "Buy shares in which company [A-" << static_cast<char>('A' + companies.size() - 1) << "]: ";
        cin >> choiceChar;
        choiceChar = toupper(choiceChar); // Convert to uppercase

        int choice = choiceChar - 'A'; // Convert character to index

        // Check if choice is valid
        if (choice < 0 || choice >= companies.size()) {
            cout << "Invalid choice! Please choose in the range A-" << static_cast<char>('A' + companies.size() - 1) << "\n";
            continue; // Go back to the start of the loop
        }

        Company* chosenCompany = companies[choice];

        // Check if the company has any shares left
        if (chosenCompany->getShares() == 0) {
            cout << "Sorry! " << chosenCompany->getName() << " has no shares available.\n";
            continue;
        }

        // Ask user how many shares they want to buy
        int sharesToBuy;
        cout << "How many shares to buy (1-" << chosenCompany->getShares() << "): ";
        cin >> sharesToBuy;

        if (!player.canBuyShares(*chosenCompany, sharesToBuy)) {
            cout << "Insufficient amount of money to buy shares.\n";
            cout << "Do you want to buy more shares? (Y/N): ";
            char continueAfterInsufficientFunds;
            cin >> continueAfterInsufficientFunds;
            if (continueAfterInsufficientFunds == 'N' || continueAfterInsufficientFunds == 'n') {
                stillBuying = false;
            }
            continue; // Go back to the start of the loop
        }

        player.buyShares(*chosenCompany, sharesToBuy); // update the player's shares
        chosenCompany->setShares(chosenCompany->getShares()); //update the company's available shares

        char continueBuying;
        cout << "Do you want to buy more shares? (Y/N): " << endl;
        cin >> continueBuying;

        if (continueBuying == 'N' || continueBuying == 'n') {
            stillBuying = false;
        }
    }
}

void sellShares(Player& player) {
    // Check if the player owns any shares
    if (player.getTotalSharesOwned() == 0) {
        cout << "You do not own any shares to sell." << endl;
        return;
    }

    bool stillSelling = true;
    while (stillSelling) {
        int companiesWithShares = 0; // Counter for companies the player has shares in
        // Display companies where the player owns shares
        cout << "Companies where you own shares:\n";
        for (int i = 0; i < companies.size(); i++) {
            int ownedShares = player.getSharesOwnedForCompany(companies[i]->getName());
            if (ownedShares > 0) {
                cout << companies[i]->getName() << " - Owned shares: " << ownedShares
                     << " - Share price: $" << companies[i]->getSharePrice() << "\n";
                companiesWithShares++;
            }
        }

        if (companiesWithShares == 0) {
            cout << "You have sold all your shares. Returning to main menu." << endl;
            break;
        }

        // Get user choice
        char choiceChar;
        cout << "Sell shares in which company: ";
        cin >> choiceChar;
        choiceChar = toupper(choiceChar); // Convert to uppercase

        // Find the chosen company in the companies vector
        Company* chosenCompany = nullptr;
        for (auto& company : companies) {
            if (company->getName()[1] == choiceChar) {
                chosenCompany = company;
                break;
            }
        }

        if (!chosenCompany) {
            cout << "Invalid choice. Please choose a valid company." << endl;
            continue;
        }

        int ownedShares = player.getSharesOwnedForCompany(chosenCompany->getName());
        if (ownedShares == 0) {
            cout << "You don't own any shares in the chosen company." << endl;
            continue; // Go back to the start of the loop
        }

        // Ask user how many shares they want to sell
        int sharesToSell;
        cout << "How many shares to sell (1-" << ownedShares << "): ";
        cin >> sharesToSell;

        if (sharesToSell < 1 || sharesToSell > ownedShares) {
            cout << "Invalid number of shares to sell.\n";
            continue; // Go back to the start of the loop
        }

        player.sellShares(*chosenCompany, sharesToSell);
        chosenCompany->setShares(chosenCompany->getShares() + sharesToSell); // Correctly update the company's available shares

        cout << "Do you want to sell more shares? (Y/N): ";
        char continueSelling;
        cin >> continueSelling;
        if (continueSelling == 'N' || continueSelling == 'n') {
            stillSelling = false;
        }
    }
}

void acquireCompany(Player& player) {
    // Check if the player has any power uses left
    if (player.getPowerUsesLeft() <= 0) {
        cout << "You have no power uses left." << endl;
        return;
    }

    // Display available companies
    cout << "Available companies to acquire:\n";
    for (int i = 0; i < companies.size(); i++) {
        if (companies[i]->isAcquired()) {
            continue; // Skip already acquired companies
        }
        char companyChar = 'A' + i; // Convert index to character
        cout << companyChar << ". " << companies[i]->getName() << " - Cost: " << companies[i]->getCost() << " shares\n";
    }

    // Get user choice
    char choiceChar;
    cout << "Acquire which company [A-" << static_cast<char>('A' + companies.size() - 1) << "]: ";
    cin >> choiceChar;
    choiceChar = toupper(choiceChar); // Convert to uppercase

    int choice = choiceChar - 'A'; // Convert character to index

    // Check if choice is valid
    if (choice < 0 || choice >= companies.size() || companies[choice]->isAcquired()) {
        cout << "Invalid choice! Please choose an available company in the range A-" << static_cast<char>('A' + companies.size() - 1) << "\n";
        return;
    }

    Company* chosenCompany = companies[choice];

    // Check if player can acquire the company
    if (player.getSharesOwnedForCompany(chosenCompany->getName()) < chosenCompany->getCost()) {
        cout << "You do not have enough shares to acquire this company." << endl;
        return;
    }

    // Acquire the company
    if (player.acquireCompany(*chosenCompany)) {
        chosenCompany->setOwner(player.getName());
        cout << "You have successfully acquired " << chosenCompany->getName() << "!" << endl;
    } else {
        cout << "Failed to acquire " << chosenCompany->getName() << "." << endl;
    }
}

void useCorporatePower(Player& player) {
    // Check if the player has any power uses left
    if (player.getPowerUsesLeft() <= 0) {
        cout << "You have no power uses left." << endl;
        return;
    }

    // Display companies the player owns
    const auto& acquiredCompanies = player.getAcquiredCompanies();
    if (acquiredCompanies.empty()) {
        cout << "You don't own any companies to use a power." << endl;
        return;
    }

    cout << "Available companies to use power from:\n";
    for (const auto& companyName : acquiredCompanies) {
        cout << companyName.substr(0, 3) << companyName.substr(4) << endl; // Format "[A] Company Name"
    }

    // Get user choice
    char choice;
    cout << "Choose a company to use its power: ";
    cin >> choice;
    choice = toupper(choice); // Convert to uppercase for consistency

    string chosenCompanyName;
    bool validChoice = false;
    for (const auto& companyName : acquiredCompanies) {
        if (companyName[1] == choice) { // Check the character inside []
            chosenCompanyName = companyName;
            validChoice = true;
            break;
        }
    }

    if (!validChoice) {
        cout << "Invalid choice. Please choose a valid company." << endl;
        return;
    }

    // Find the chosen company in the companies vector
    Company* chosenCompany = nullptr;
    for (auto& company : companies) {
        if (company->getName() == chosenCompanyName) {
            chosenCompany = company;
            break;
        }
    }

    string power = chosenCompany->getPower();

    // Calculate the multiplier
    int multiplier = rand() % player.getDifficultyLevel() + 2;

    if (power == "+ money") {
        double moneyToAdd = 10 * (player.getDifficultyLevel() * multiplier);
        player.setMoney(player.getMoney() + moneyToAdd);
        cout << "Added $" << moneyToAdd << " to your account." << endl;
    } else if (power == "+ shares") {
        int sharesToAdd = player.getDifficultyLevel() * multiplier;
        player.addShares(sharesToAdd);
        cout << "Added " << sharesToAdd << " shares to " << chosenCompanyName << "." << endl;
    } else if (power == "+ assets") {
        double moneyToAdd = 10 * (player.getDifficultyLevel() * multiplier);
        player.setMoney(player.getMoney() + moneyToAdd);
        int sharesToAdd = player.getDifficultyLevel() * multiplier;
        player.addShares(sharesToAdd);
        cout << "Added $" << moneyToAdd << " to your account and " << sharesToAdd << " shares to " << chosenCompanyName << "." << endl;
    }

    // Deduct a power use
    player.setPowerUsesLeft(player.getPowerUsesLeft() - 1);
}


void quitGame() {
    cout << "Are you sure you want to quit, " << player.getName() << "? (Y/N): ";
    char userAction;
    cin >> userAction;

    if (userAction == 'Y' || userAction == 'y') {
        cout << "Thank you for playing Risky Business!" << endl;
        exit(0);
    }
}

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
