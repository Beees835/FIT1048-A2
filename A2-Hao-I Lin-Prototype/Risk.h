//
// Created by Hao-I Lin on 11/09/2023.
//

#include <string>
using namespace std;

#ifndef A2_HAO_I_LIN_PROTOTYPE_RISK_H
#define A2_HAO_I_LIN_PROTOTYPE_RISK_H

#include <string>
using namespace std;

class Risk {
private:
    string description;
    string effect;
    int minValue;
    int maxValue;

public:
    // Constructors
    Risk(); // Default constructor
    Risk(string description, string effect, int minValue, int maxValue); // Overloaded constructor

    // Accessors
    string getDescription();
    string getEffect();
    int getMinValue();
    int getMaxValue();

    // Mutators
    void setDescription(string description);
    void setEffect(string effect);
    void setMinValue(int minValue);
    void setMaxValue(int maxValue);

    // Display details
    string getDetails();

    // Test function
    static void testRisk();
};


#endif //A2_HAO_I_LIN_PROTOTYPE_RISK_H

