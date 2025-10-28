#include <iostream>
#include <string>
#include <fstream>
#include <unordered_map>
#include <algorithm>
#include <iomanip>
#include <cstdlib>
#include <clocale>
#include <cstring> 
#include "Pipe.h"
#include "cs.h"
#include "PipeManager.h"
#include "CSManager.h"
#include "Utilities.h"
#include "FileOperations.h"
#include "SearchMenus.h"

std::ofstream logFile("log.txt", std::ios::app);

int main() {
    PipeManager pipeManager;
    CSManager csManager;

    setlocale(LC_ALL, "");

    std::string choice;
    long choiceint;

    logFile << "=== PROGRAM STARTED ===" << std::endl;

    while (true) {
        menuDisplay();
        std::cout << "Choose command: ";
        std::getline(std::cin, choice);

        choiceint = getChoice(choice);

        switch (choiceint) {
        case 1:
            pipeManager.addPipe();
            break;
        case 2:
            csManager.addCS();
            break;
        case 3:
            pipeManager.displayAllPipes();
            csManager.displayAllCS();
            break;
        case 4:
            searchPipeMenu(pipeManager);
            break;
        case 5:
            searchCSMenu(csManager);
            break;
        case 6:
            saveToFile(pipeManager, csManager);
            break;
        case 7:
            loadFromFile(pipeManager, csManager);
            break;
        case 0:
            std::cout << "Exiting program." << std::endl;
            logFile << "=== PROGRAM FINISHED ===" << std::endl;
            logFile.close();
            return 0;
        default:
            std::cout << "Invalid choice, try again." << std::endl;
            continue;
        }
    }
}