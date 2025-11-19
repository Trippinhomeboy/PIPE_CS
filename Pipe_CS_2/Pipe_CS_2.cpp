#include "PipeManager.h"
#include "CSManager.h"
#include "WebManager.h"
#include "FileOperations.h"
#include "SearchMenus.h"
#include "Utilities.h"
#include <iostream>
#include <string>
#include <fstream>

std::ofstream logFile("log.txt");

int main() {
    PipeManager pipeManager;
    CSManager csManager;
    WebManager webManager;

    while (true) {
        menuDisplay();
        std::string command;
        std::getline(std::cin, command);

        if (!command.empty()) {
            logFile << command << std::endl;
        }

        long choice = getChoice(command);
        switch (choice) {
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
        case 13:
            webManager.calculateMaxFlow(pipeManager, csManager);
            break;
        case 14:
            webManager.findShortestPath(pipeManager, csManager);
            break;
        case 0:
            std::cout << "Exiting program." << std::endl;
            logFile.close();
            return 0;
        default:
            std::cout << "Invalid choice. Please try again." << std::endl;
        }
    }

    logFile.close();
    return 0;
}