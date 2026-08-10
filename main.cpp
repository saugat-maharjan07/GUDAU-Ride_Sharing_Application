#include <iostream>
#include <iomanip>
#include <limits>
#include <string>

using namespace std;

// Detects Empty Line
string readNonEmptyLine(const string &prompt) {
    string value;
    while (true) {
        cout << prompt;
        getline(cin, value);
        if (!value.empty()) break;
        cout << "  -> This field cannot be empty. Please try again.\n";
    }
    return value;
}

//