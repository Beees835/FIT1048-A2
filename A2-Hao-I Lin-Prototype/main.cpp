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

    // Get player names and set difficulty level for each player using overloaded constructor
    for(int i = 0; i < numPlayers; i++){
        string name;
        cout << "Enter player " << i+1 << "'s name: ";
        cin >> name;
        Player player(name, difficulty);
        players.push_back(player);
    }

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

        while (getline(file, line) && companyCount < maxCompanies) {
            stringstream ss(line);
            int index;
            string name, power;
            char delimiter;

            ss >> index >> delimiter;
            getline(ss, name, ';');
            getline(ss, power, ';');

            Company company = Company(name, index);
            companies.push_back(company);

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
        for (int i = 0; i < numPlayers; ++i) {
            char userAction;
            cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
            cout << "         [B]uy   [S]ell   [A]cquire   [P]ower   [R]isk   [Q]uit              " << endl;
            cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
            cout << "Player " << (i + 1) << "'s turn:\n";
            cout << "What will you do now ? " << endl;
            cin >> userAction;
            switch (userAction) {
                case 'B' | 'b':
                    buyShares(players[i]);
                    displayInterface(currentSettings);  // Return to main interface after buying
                    break;
                case 'S' | 's':
                    // Sell shares
                    break;
                case 'A' | 'a':
                    // Acquire company
                    break;
                case 'P' | 'p':
                    // Use corporate power
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
        cout << left << setw(20) << "Company Names" << setw(20) << "Shares" << setw(20) << "Power" << endl;
        for (const auto& companyDetail : player.getCompanyDetails()) {
//            string companyName = companyDetail.first;
//            int sharesOwned = companyDetail.second;
//            cout << left << setw(30) << companyName << setw(20) << sharesOwned << setw(20) << "No" << endl;
        }
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

void buyShares(Player& player) {
    bool stillBuying = true;
    while (stillBuying) {
        // Display available companies
        cout << "Available companies:\n";
        for (int i = 0; i < companies.size(); i++) {
            char companyChar = 'A' + i; // Convert index to character
            cout << companyChar << ". " << companies[i].getName() << " - Available shares: " << companies[i].getShares()
                 << " - Share price: $" << companies[i].getSharePrice() << "\n";
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

        Company &chosenCompany = companies[choice];

        // Ask user how many shares they want to buy
        int sharesToBuy;
        cout << "How many shares to buy (1-" << chosenCompany.getShares() << "): ";
        cin >> sharesToBuy;

        if (!player.canBuyShares(chosenCompany, sharesToBuy)) {
            cout << "Insufficient amount of money to buy shares.\n";
            cout << "Do you want to buy more shares? (Y/N): ";
            char continueAfterInsufficientFunds;
            cin >> continueAfterInsufficientFunds;
            if (continueAfterInsufficientFunds == 'N' || continueAfterInsufficientFunds == 'n') {
                stillBuying = false;
            }
            continue; // Go back to the start of the loop
        }

        player.buyShares(chosenCompany, sharesToBuy);
        chosenCompany.setShares(chosenCompany.getShares() - sharesToBuy); // Update the company's available shares

        char continueBuying;
        cout << "Do you want to buy more shares? (Y/N): " << endl;
        cin >> continueBuying;

        if (continueBuying == 'N' || continueBuying == 'n') {
            stillBuying = false;
        }
    }
}


void quitGame() {
    // allow player to quit game at anytime
    cout << "Quit game now? (Y/N): ";
    char userAction;
    cin >> userAction;
    if (userAction == 'Y' || userAction == 'y') {
        cout << "Thank you for playing Risky Business!" << endl;
        exit(0);
    }
    else {
        return;
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
