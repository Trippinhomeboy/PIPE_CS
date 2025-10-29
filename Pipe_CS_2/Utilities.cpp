#include "Utilities.h"
#include <iostream>
#include <iomanip>

void menuDisplay() {
    std::cout << std::endl;
    std::cout << std::setw(30) << std::setfill('=') << "" << std::endl;
    std::cout << std::setw(17) << std::setfill(' ') << " Menu:" << std::endl;
    std::cout << std::setw(30) << std::setfill('=') << "" << std::endl;

    std::cout << "1) Add pipe" << std::endl;
    std::cout << "2) Add compressor station" << std::endl;
    std::cout << "3) View all objects" << std::endl;
    std::cout << "4) Search and manage (pipes)" << std::endl;
    std::cout << "5) Search and manage (stations)" << std::endl;
    std::cout << "6) Save" << std::endl;
    std::cout << "7) Load" << std::endl;
    std::cout << "0) Exit" << std::endl;

    std::cout << std::setw(30) << std::setfill('=') << "" << std::endl;
    std::cout << std::endl;
}