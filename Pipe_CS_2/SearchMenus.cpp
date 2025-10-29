#include "SearchMenus.h"
#include "Utilities.h"
#include <iostream>
#include <string>

void searchPipeMenu(PipeManager& pipeManager) {
    while (true) {
        std::cout << "=== PIPE SEARCH AND MANAGEMENT ===" << std::endl;
        std::cout << "1) Search by name" << std::endl;
        std::cout << "2) Search by status" << std::endl;
        std::cout << "3) View all pipes" << std::endl;
        std::cout << "4) Edit pipe (by ID)" << std::endl;
        std::cout << "5) Delete pipe (by ID)" << std::endl;
        std::cout << "0) Back" << std::endl;

        int choice = inputInRange<int>("Choose action: ", 0, 5);

        std::unordered_map<int, Pipe> searchResult;

        switch (choice) {
        case 1: {
            std::cout << "Enter name for search: ";
            std::string name;
            std::getline(std::cin, name);
            searchResult = pipeManager.searchByName(name);
            if (!searchResult.empty()) {
                pipeManager.displayPipes(searchResult);

                std::cout << "1) Edit all found pipes" << std::endl;
                std::cout << "2) Delete all found pipes" << std::endl;
                std::cout << "0) Back" << std::endl;

                int action = inputInRange<int>("Choose action: ", 0, 2);
                switch (action) {
                case 1:
                    pipeManager.editPipeSubset(searchResult);
                    std::cout << "Status of all found pipes changed." << std::endl;
                    break;
                case 2:
                    pipeManager.deletePipeSubset(searchResult);
                    std::cout << "All found pipes deleted." << std::endl;
                    break;
                case 0:
                    break;
                }
            }
            else {
                std::cout << "No pipes found matching criteria." << std::endl;
            }
            break;
        }
        case 2: {
            int statusChoice = inputInRange<int>("Choose status (1 - in repair, 0 - not in repair): ", 0, 1);
            searchResult = pipeManager.searchByStatus(statusChoice == 1);
            if (!searchResult.empty()) {
                pipeManager.displayPipes(searchResult);

                std::cout << "1) Edit all found pipes" << std::endl;
                std::cout << "2) Delete all found pipes" << std::endl;
                std::cout << "0) Back" << std::endl;

                int action = inputInRange<int>("Choose action: ", 0, 2);
                switch (action) {
                case 1:
                    pipeManager.editPipeSubset(searchResult);
                    std::cout << "Status of all found pipes changed." << std::endl;
                    break;
                case 2:
                    pipeManager.deletePipeSubset(searchResult);
                    std::cout << "All found pipes deleted." << std::endl;
                    break;
                case 0:
                    break;
                }
            }
            else {
                std::cout << "No pipes found matching criteria." << std::endl;
            }
            break;
        }
        case 3:
            pipeManager.displayAllPipes();
            break;
        case 4:
            pipeManager.editPipe();
            break;
        case 5:
            pipeManager.deletePipe();
            break;
        case 0:
            return;
        }
    }
}

void searchCSMenu(CSManager& csManager) {
    while (true) {
        std::cout << "=== COMPRESSOR STATION SEARCH AND MANAGEMENT ===" << std::endl;
        std::cout << "1) Search by name" << std::endl;
        std::cout << "2) Search by workshop percentage" << std::endl;
        std::cout << "3) View all stations" << std::endl;
        std::cout << "4) Edit station (by ID)" << std::endl;
        std::cout << "5) Delete station (by ID)" << std::endl;
        std::cout << "0) Back" << std::endl;

        int choice = inputInRange<int>("Choose action: ", 0, 5);

        std::unordered_map<int, CompressorStation> searchResult;

        switch (choice) {
        case 1: {
            std::cout << "Enter name for search: ";
            std::string name;
            std::getline(std::cin, name);
            searchResult = csManager.searchByName(name);
            if (!searchResult.empty()) {
                csManager.displayCS(searchResult);

                std::cout << "1) Edit all found stations" << std::endl;
                std::cout << "2) Delete all found stations" << std::endl;
                std::cout << "0) Back" << std::endl;

                int action = inputInRange<int>("Choose action: ", 0, 2);
                switch (action) {
                case 1:
                    csManager.editCSSubset(searchResult);
                    std::cout << "All found stations edited." << std::endl;
                    break;
                case 2:
                    csManager.deleteCSSubset(searchResult);
                    std::cout << "All found stations deleted." << std::endl;
                    break;
                case 0:
                    break;
                }
            }
            else {
                std::cout << "No stations found matching criteria." << std::endl;
            }
            break;
        }
        case 2: {
            double minPercent = inputInRange<double>("Enter minimum workshop percentage: ", 0.0, 100.0);
            double maxPercent = inputInRange<double>("Enter maximum workshop percentage: ", minPercent, 100.0);
            searchResult = csManager.searchByWorkshopPercentage(minPercent, maxPercent);
            if (!searchResult.empty()) {
                csManager.displayCS(searchResult);

                std::cout << "1) Edit all found stations" << std::endl;
                std::cout << "2) Delete all found stations" << std::endl;
                std::cout << "0) Back" << std::endl;

                int action = inputInRange<int>("Choose action: ", 0, 2);
                switch (action) {
                case 1:
                    csManager.editCSSubset(searchResult);
                    std::cout << "All found stations edited." << std::endl;
                    break;
                case 2:
                    csManager.deleteCSSubset(searchResult);
                    std::cout << "All found stations deleted." << std::endl;
                    break;
                case 0:
                    break;
                }
            }
            else {
                std::cout << "No stations found matching criteria." << std::endl;
            }
            break;
        }
        case 3:
            csManager.displayAllCS();
            break;
        case 4:
            csManager.editCS();
            break;
        case 5:
            csManager.deleteCS();
            break;
        case 0:
            return;
        }
    }
}