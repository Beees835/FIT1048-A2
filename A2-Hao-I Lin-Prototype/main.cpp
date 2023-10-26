/**************************************************************************
 * project name: Risky Business Simulation Game prototype
 * Author: Hao-I Lin
 * Purpose: This program is a prototype of Risky Business Simulation Game.
 **************************************************************************/

#include <iostream>
#include <fstream>
#include <filesystem>
#include <sstream>
#include <limits>
#include <iomanip>
#include <set>
#include "Player.h"
#include "Company.h"
#include "Risk.h"
#include "main.h"
#include "bronzeCompany.h"
#include "silverCompany.h"
#include "goldCompany.h"


using namespace std;

//////////////////
// Declarations //
//////////////////
std::vector<Company*> companies;  // Using pointers to store company objects
Difficulty difficulty;
int currentMaxDay = 1;
Player player;

///////////////////////
//  Testing methods  //
///////////////////////
void testingMergeCompany(Player& player) {
    if (companies.size() == 0) {
        cout << "No companies available to merge.\n";
        return;
    }
    Company* targetCompany = companies[0];
    cout << "Selected company for merge: " << targetCompany->getName() << "\n";

    string opponentName = "DummyOpponent";
    cout << "Selected opponent for merge: " << opponentName << "\n";

    if (player.getSharesOwnedForCompany(targetCompany->getName()) == 0) {
        cout << "You don't have shares in the selected company.\n";
        return;
    }

    double totalCost = targetCompany->getSharePrice() * player.getSharesOwnedForCompany(targetCompany->getName());
    if (player.getMoney() < totalCost) {
        cout << "You can't afford the merger.\n";
        return;
    }

    player.setMoney(player.getMoney() - totalCost);
    int opponentShares = 10;  // Example value

    // Use buyShares method to simulate acquiring the shares from the opponent
    player.buyShares(*targetCompany, opponentShares);

    // Update the company's available shares
    targetCompany->setShares(targetCompany->getShares());

    cout << "Merge successful! You now have " << player.getSharesOwnedForCompany(targetCompany->getName()) << " shares in " << targetCompany->getName() << ".\n";
}

void initializeLoadedGameState() {
    // Check if any companies were acquired and update their status
    for (auto& companyPtr : companies) {
        string companyName = companyPtr->getName();

        // Update the company's status if it has been acquired by the player
        if (player.hasAcquiredCompany(companyName)) {
            companyPtr->setAcquired(true); // Mark the company as acquired
            companyPtr->setOwner(player.getName()); // Set the player as the owner
        } else {
            companyPtr->setAcquired(false); // Mark the company as not acquired
            companyPtr->setOwner("Nobody"); // Reset owner if the player doesn't own the company
        }

        // Adjust available shares based on player's ownership
        int sharesOwned = player.getSharesOwnedForCompany(companyName);
        if (sharesOwned > 0) {
            companyPtr->removeShares(sharesOwned);
        }
    }
}


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
    Risk::loadRisksFromFile("riskAdvanced.txt");

    ifstream checkSave("game_save.txt");
    if (checkSave) {
        checkSave.close(); // Close the file after checking its existence
        char choice;
        cout << "A saved game has been detected. Do you want to load it? (Y/N): ";
        cin >> choice;
        if (choice == 'Y' || choice == 'y') {
            if (loadGameState()) {
                initializeLoadedGameState();
                loadCompanyDetail();
                return;
            } else {
                cout << "Failed to load the game. Starting a new game." << endl;
            }
        }
    }

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
    loadCompanyDetail();
}

void loadCompanyDetail() {
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
                newCompany = new bronzeCompany(name, index);
            } else if (power == "+ shares") {
                newCompany = new silverCompany(name, index);
            } else if (power == "+ assets") {
                newCompany = new goldCompany(name, index);
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
        cout << "     [B]uy   [S]ell   [A]cquire   [P]ower   [R]isk   [M]erge   Sa[V]e   [Q]uit     " << endl;
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
                takeRisk(player, difficulty);
                displayInterface(currentSettings);
                break;
            case 'Q' | 'q':
                quitGame();
                displayInterface(currentSettings);
                break;
            case 'M' | 'm':
                testingMergeCompany(player);
                displayInterface(currentSettings);
                break;
            case 'V' | 'v':
                saveGameState();
                displayInterface(currentSettings);
                break;
            default:
                cout << "Invalid input!\n";
                break;
        }
        // update current day
        currentMaxDay++;
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
    // update the company share prices after player finished action
    for (auto& company : companies) {
        company->updateSharePrice();
    }

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

    // Display all companies, not just the ones owned by the player
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
        cout << companyChar << ". " << companies[i]->getName() << " - Total Shares: " << companies[i]->getMaxShares() << "\n";
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
    if (player.getSharesOwnedForCompany(chosenCompany->getName()) < chosenCompany->getMaxShares()) {
        cout << "You do not own all the shares of this company. Owning all shares is required to acquire it." << endl;
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
    bool validChoice = false;

    while (!validChoice) {
        cout << "Choose a company to use its power: ";
        cin >> choice;
        if (cin.fail()) {
            cin.clear();  // Clear the error flag
            cin.ignore(numeric_limits<streamsize>::max(), '\n');  // Discard invalid input
            continue;
        }

        choice = toupper(choice); // Convert to uppercase for consistency

        for (const auto& companyName : acquiredCompanies) {
            if (companyName[1] == choice) { // Check the character inside []
                validChoice = true;
                break;
            }
        }

        if (!validChoice) {
            cout << "Invalid choice. Please choose a valid company." << endl;
        }
    }

    string chosenCompanyName;
    // Display acquired companies and companies where shares are owned
    for (const auto& companyName : acquiredCompanies) {
        if (companyName[1] == choice) {
            chosenCompanyName = companyName;
            break;
        }
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

    // based on the return string of getPower(), do the following action
    // reason doing this is because rather than spreading around the logic
    // we just organize all logics here ,and we don't need to reply on the
    // derived classes to implement the logic. Since keep the logic in the
    // derived classes is not a good idea, the code becomes harder to manage
    // and maintain.
    if (power == "+ money") {
        double moneyToAdd = 10 * (player.getDifficultyLevel() * multiplier);
        player.setMoney(player.getMoney() + moneyToAdd);
        cout << "Added $" << moneyToAdd << " to your account." << endl;
    } else if (power == "+ shares") {
        int sharesToAdd = player.getDifficultyLevel() * multiplier;
        player.addShares(sharesToAdd);
        player.addSharesForCompany(chosenCompanyName, sharesToAdd); // Add shares for the company
        cout << "Added " << sharesToAdd << " shares to " << chosenCompanyName << "." << endl;
    } else if (power == "+ assets") {
        double moneyToAdd = 10 * (player.getDifficultyLevel() * multiplier);
        player.setMoney(player.getMoney() + moneyToAdd);
        int sharesToAdd = player.getDifficultyLevel() * multiplier;
        player.addShares(sharesToAdd);
        player.addSharesForCompany(chosenCompanyName, sharesToAdd); // Add shares for the company
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

void takeRisk(Player& player, Difficulty difficulty) {
    if (!Risk::areRisksLoaded()) {
        cerr << "Error: No risks loaded. Please load risks first." << endl;
        return;
    }

    Risk randomRisk = Risk::getRandomRisk();
    cout << "You drew the following risk:\n";
    cout << randomRisk.getDetails() << endl;

    int gameModeMultiplier = static_cast<int>(difficulty) + 1;
    int effectValue = 0;

    if (randomRisk.getMaxValue() > 0) {
        effectValue = gameModeMultiplier * (rand() % (randomRisk.getMaxValue() - randomRisk.getMinValue() + 1) + randomRisk.getMinValue());
    } else if (randomRisk.getMinValue() < 0) {
        effectValue = randomRisk.getMaxValue() * gameModeMultiplier;
    }

    if (randomRisk.getEffect() == "money") {
        player.setMoney(player.getMoney() + effectValue);
        cout << "Your money has been adjusted by: $" << effectValue << endl;
    } else if (randomRisk.getEffect() == "shares") {
        // Check if the player owns any shares
        if (player.getTotalSharesOwned() == 0) {
            cout << "The risk affects shares, but you don't own shares in any company!" << endl;
            return;
        }

        // Display companies where the player owns shares
        cout << "Companies where you own shares:\n";
        char choiceChar = 'A';
        for (const auto& companyPtr : companies) {
            int ownedShares = player.getSharesOwnedForCompany(companyPtr->getName());
            if (ownedShares > 0) {
                cout << choiceChar << ". " << companyPtr->getName() << " - Owned shares: " << ownedShares << "\n";
                choiceChar++;
            }
        }

        // Get user choice
        char userChoice;
        cout << "Choose a company to apply the risk effect on: ";
        cin >> userChoice;
        userChoice = toupper(userChoice); // Convert to uppercase

        Company* chosenCompany = nullptr;
        int index = userChoice - 'A'; // Convert character to index
        for (const auto& companyPtr : companies) {
            if (player.getSharesOwnedForCompany(companyPtr->getName()) > 0 && index-- == 0) {
                chosenCompany = companyPtr;
                break;
            }
        }

        if (!chosenCompany) {
            cout << "Invalid choice. Returning to the main menu." << endl;
            return;
        }

        if (effectValue > 0) {
            player.buyShares(*chosenCompany, effectValue);
        } else {
            player.sellShares(*chosenCompany, abs(effectValue));
        }
        cout << "Your shares in " << chosenCompany->getName() << " have been adjusted by: " << effectValue << endl;

    } else if (randomRisk.getEffect() == "assets") {
        player.setMoney(player.getMoney() + effectValue);
        cout << "Your money has been adjusted by: $" << effectValue << endl;

        // Now calculate the effect on shares, similar to the 'shares' effect above
        if (player.getTotalSharesOwned() > 0) {
            cout << "Companies where you own shares:\n";
            char choiceChar = 'A';
            for (const auto& companyPtr : companies) {
                int ownedShares = player.getSharesOwnedForCompany(companyPtr->getName());
                if (ownedShares > 0) {
                    cout << choiceChar << ". " << companyPtr->getName() << " - Owned shares: " << ownedShares << "\n";
                    choiceChar++;
                }
            }

            char userChoice;
            cout << "Choose a company to apply the risk effect on shares: ";
            cin >> userChoice;
            userChoice = toupper(userChoice);

            Company* chosenCompany = nullptr;
            int index = userChoice - 'A';
            for (const auto& companyPtr : companies) {
                if (player.getSharesOwnedForCompany(companyPtr->getName()) > 0 && index-- == 0) {
                    chosenCompany = companyPtr;
                    break;
                }
            }

            if (!chosenCompany) {
                cout << "Invalid choice. Returning to the main menu." << endl;
                return;
            }

            if (effectValue > 0) {
                player.buyShares(*chosenCompany, effectValue);
            } else {
                player.sellShares(*chosenCompany, abs(effectValue));
            }

            cout << "Both your money and shares in " << chosenCompany->getName() << " have been adjusted." << endl;
        } else {
            cout << "The risk affects shares, but you don't own shares in any company!" << endl;
        }
    }
}


void mergeCompany(Player& player) {
    // Ask the player which company they want to merge.
    cout << "Which company do you want to merge with?\n";
    for (int i = 0; i < companies.size(); i++) {
        cout << char('A' + i) << ". " << companies[i]->getName() << "\n";
    }

    char companyChoice;
    cin >> companyChoice;
    companyChoice = toupper(companyChoice);
    int companyIndex = companyChoice - 'A';

    if (companyIndex < 0 || companyIndex >= companies.size()) {
        cout << "Invalid choice.\n";
        return;
    }

    Company* targetCompany = companies[companyIndex];

    // Ask the player which opponent player they want to merge with
    // Note: For the sake of this example, let's assume there is only one opponent named "Opponent".
    // In a real game, you'd need to manage multiple players and list them out for selection.
    cout << "Which opponent do you want to merge with? (Enter 'Opponent' for now): ";
    string opponentName;
    cin >> opponentName;

    if (opponentName != "Opponent") {
        cout << "Invalid opponent name.\n";
        return;
    }

    // Check that both players have shares in the selected company.
    // Note: In this example, we assume the opponent has shares in every company.
    if (player.getSharesOwnedForCompany(targetCompany->getName()) == 0) {
        cout << "You don't have shares in the selected company.\n";
        return;
    }

    // The player initiating the merge must pay the opponent the current value of the shares.
    double totalCost = targetCompany->getSharePrice() * player.getSharesOwnedForCompany(targetCompany->getName());
    if (player.getMoney() < totalCost) {
        cout << "You can't afford the merge.\n";
        return;
    }

    // Complete the transactions
    player.setMoney(player.getMoney() - totalCost);
    // Note: We assume the opponent gives all their shares in the company.
    // In a real game, you'd need to get the exact number of shares from the opponent.
    int opponentShares = 10;  // Example value
    player.addShares(opponentShares);
    cout << "Merge successful! You now have " << player.getSharesOwnedForCompany(targetCompany->getName()) << " shares in " << targetCompany->getName() << ".\n";
}


void saveGameState() {
    ofstream outFile("game_save.txt");
    if (!outFile) {
        cout << "Error: Unable to save the game." << endl;
        return;
    } else {
        cout << "Game saved successfully." << endl;
    }

    // Saving player data
    outFile << player.getName() << "\n";
    outFile << player.getTotalSharesOwned() << " "
            << player.getMoney() << " "
            << player.getPowerUsesLeft() << "\n";

    // Saving companies data
    for (const auto& companyPtr : companies) {
        int sharesOwnedByPlayer = player.getSharesOwnedForCompany(companyPtr->getName());

        outFile << companyPtr->getName() << "\n"
                << companyPtr->getSharePrice() << " "
                << companyPtr->getShares() << " "
                << sharesOwnedByPlayer << " "
                << companyPtr->getOwner() << "\n"
                << companyPtr->getPower() << "\n";
    }

    // Saving shares owned by player
    for (const auto& companyPtr : companies) {
        int sharesOwnedByPlayer = player.getSharesOwnedForCompany(companyPtr->getName());
        if (sharesOwnedByPlayer > 0) {
            outFile << companyPtr->getName() << " " << sharesOwnedByPlayer << "\n";
        }
    }

    outFile.close();
}

bool loadGameState() {
    ifstream inFile("game_save.txt");
    if (!inFile) {
        cout << "Error: Unable to load the game." << endl;
        return false;
    }

    // Loading player data
    string playerName;
    getline(inFile, playerName);
    player.setName(playerName);

    int totalShares, powerUses;
    double money;
    inFile >> totalShares >> money >> powerUses;
    player.setTotalSharesOwned(totalShares);
    player.setMoney(money);
    player.setPowerUsesLeft(powerUses);

    // Loading companies data
    for (auto& companyPtr : companies) {
        string companyName, owner, power;
        int sharePrice, availableShares, playerShares;

        inFile.ignore();
        getline(inFile, companyName);
        inFile >> sharePrice >> availableShares >> playerShares;
        inFile.ignore();
        getline(inFile, owner);
        getline(inFile, power);

        companyPtr->setName(companyName);
        companyPtr->setSharePrice(sharePrice);
        companyPtr->setShares(availableShares);
        companyPtr->setOwner(owner);
        companyPtr->setPower(power);

        if (playerShares > 0) {
            player.addSharesForCompany(companyName, playerShares);
        }
    }

    // Setting shares owned by player
    while (inFile) {
        string companyName;
        int shares;
        inFile >> companyName >> shares;
        if (!companyName.empty()) {
            player.addSharesForCompany(companyName, shares);
        }
    }

    inFile.close();
    return true;
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