#ifndef UTILITIES_H
#define UTILITIES_H

#include <iostream>
#include <string>
#include <sstream>
#include <limits>
#include <cctype> 
#include <fstream>
#include <cstdlib> 

extern std::ofstream logFile;

template<typename T>
T inputInRange(const std::string& prompt, T minValue, T maxValue) {
    T value;
    while (true) {
        std::cout << prompt;
        std::string input;
        std::getline(std::cin, input);

        if (!input.empty()) {
            logFile << input << std::endl;
        }

        std::stringstream ss(input);
        if (ss >> value && ss.eof()) {
            if (value >= minValue && value <= maxValue) {
                return value;
            }
            else {
                std::cout << "Error: enter a number from " << minValue << " to " << maxValue << "." << std::endl;
            }
        }
        else {
            std::cout << "Error: enter a valid number." << std::endl;
        }
    }
}

inline bool isValidChoice(const std::string& s) {
    if (s.empty()) return false;
    for (char c : s) {
        if (!std::isdigit(static_cast<unsigned char>(c))) return false;
    }
    return s.length() < 2;
}

inline long getChoice(const std::string& command) {
    if (isValidChoice(command)) {
        return std::strtol(command.c_str(), NULL, 10);
    }
    return -1;
}

void menuDisplay();

#endif