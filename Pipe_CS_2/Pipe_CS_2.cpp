#include <iostream>
#include <string>
#include <fstream>
#include "Pipe.h"
#include "cs.h"
#include "PipeManager.h"
#include "CSManager.h"
#include "Utilities.h"
#include "FileOperations.h"
#include "SearchMenus.h"
#include "WebManager.h"

std::ofstream logFile("log.txt", std::ios::app);

int main() {
    PipeManager pipeManager;
    CSManager csManager;
    WebManager webManager;

    std::string choice;

    while (true) {
        menuDisplay();
        std::cout << "Choose command: ";
        std::getline(std::cin, choice);

        long choiceint = getChoice(choice);

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
            searchPipeMenu(pipeManager, webManager.getNetwork());
            break;
        case 5:
            searchCSMenu(csManager, webManager.getNetwork());
            break;
        case 6:
            saveToFile(pipeManager, csManager);
            break;
        case 7:
            loadFromFile(pipeManager, csManager);
            break;
        case 8:
            webManager.addConnection(pipeManager, csManager);
            break;
        case 9:
            webManager.removeConnection();
            break;
        case 10:
            webManager.displayConnections();
            break;
        case 11:
            webManager.topologicalSort();
            break;
        case 12:
            webManager.checkCycles();
            break;
        case 0:
            std::cout << "Exit." << std::endl;
            return 0;
        default:
            std::cout << "Invalid choice." << std::endl;
        }
    }
}