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

int Pipe::nextId = 1;
int CompressorStation::nextId = 1;
std::ofstream logFile("log.txt", std::ios::app);

// Реализация класса Pipe
Pipe::Pipe() : id(nextId++), name(""), length(0), diameter(0), status(false) {}

Pipe::Pipe(const std::string& name, double length, double diameter, bool status)
    : id(nextId++), name(name), length(length), diameter(diameter), status(status) {
}

void Pipe::editRepairStatus() {
    status = !status;
    std::cout << "Repair status changed to: " << (status ? "Yes" : "No") << std::endl;
}

void Pipe::readFromConsole() {
    std::cout << "Enter pipe name: ";
    std::getline(std::cin, name);

    length = inputInRange<double>("Enter pipe length (in km): ", 1.0, 10000.0);
    diameter = inputInRange<double>("Enter pipe diameter (in mm): ", 1.0, 10000.0);
    status = false;
}

void Pipe::writeToConsole() const {
    std::cout << std::endl;
    std::cout << "Pipe ID: " << id << std::endl;
    std::cout << "Pipe name: " << name << std::endl;
    std::cout << "Length (km): " << length << std::endl;
    std::cout << "Diameter (mm): " << diameter << std::endl;
    std::cout << "Repair status: " << (status ? "Yes" : "No") << std::endl << std::endl;
}

std::ostream& operator<<(std::ostream& out, const Pipe& pipe) {
    out << "PIPE" << std::endl;
    out << pipe.id << std::endl;
    out << pipe.name << std::endl;
    out << pipe.length << std::endl;
    out << pipe.diameter << std::endl;
    out << (pipe.status ? "1" : "0") << std::endl;
    return out;
}

std::istream& operator>>(std::istream& in, Pipe& pipe) {
    std::string line;
    std::getline(in, line); // Read pipe
    std::getline(in, line);
    pipe.id = std::stoi(line);
    std::getline(in, pipe.name);
    std::getline(in, line);
    pipe.length = std::stod(line);
    std::getline(in, line);
    pipe.diameter = std::stod(line);
    std::getline(in, line);
    pipe.status = (line == "1");
    return in;
}

std::ofstream& operator<<(std::ofstream& out, const Pipe& pipe) {
    out << "PIPE" << std::endl;
    out << pipe.id << std::endl;
    out << pipe.name << std::endl;
    out << pipe.length << std::endl;
    out << pipe.diameter << std::endl;
    out << (pipe.status ? "1" : "0") << std::endl;
    return out;
}

std::ifstream& operator>>(std::ifstream& in, Pipe& pipe) {
    std::string line;
    std::getline(in, line); // Read "PIPE"
    std::getline(in, line);
    pipe.id = std::stoi(line);
    std::getline(in, pipe.name);
    std::getline(in, line);
    pipe.length = std::stod(line);
    std::getline(in, line);
    pipe.diameter = std::stod(line);
    std::getline(in, line);
    pipe.status = (line == "1");
    return in;
}

// Реализация класса CompressorStation
CompressorStation::CompressorStation() : id(nextId++), name(""), workshops(0), workshopsInWork(0), efficiency(0) {}

CompressorStation::CompressorStation(const std::string& name, int workshops, int workshopsInWork, double efficiency)
    : id(nextId++), name(name), workshops(workshops), workshopsInWork(workshopsInWork), efficiency(efficiency) {
}

void CompressorStation::editWorkshop() {
    int command = inputInRange<int>("1 - Start workshop\n2 - Stop workshop\nChoose action: ", 1, 2);

    switch (command) {
    case 1:
        if (workshops > workshopsInWork) {
            workshopsInWork++;
            std::cout << "One more workshop started" << std::endl;
            std::cout << "Now working " << workshopsInWork << " of " << workshops << " workshops" << std::endl;
        }
        else {
            std::cout << "All workshops are already working" << std::endl;
        }
        break;
    case 2:
        if (0 < workshopsInWork) {
            workshopsInWork--;
            std::cout << "Workshop stopped" << std::endl;
            std::cout << "Now working " << workshopsInWork << " of " << workshops << " workshops" << std::endl;
        }
        else {
            std::cout << "All workshops are already stopped" << std::endl;
        }
        break;
    default:
        std::cout << "Invalid choice, try again." << std::endl;
    }
}

void CompressorStation::readFromConsole() {
    std::cout << "Enter station name: ";
    std::getline(std::cin, name);

    workshops = inputInRange<int>("Enter number of workshops: ", 1, 10000);
    workshopsInWork = inputInRange<int>("Enter number of working workshops: ", 0, workshops);
    efficiency = inputInRange<double>("Enter efficiency (in %): ", 0.01, 100.0);
}

void CompressorStation::writeToConsole() const {
    std::cout << "Station ID: " << id << std::endl;
    std::cout << "Station name: " << name << std::endl;
    std::cout << "Number of workshops: " << workshops << std::endl;
    std::cout << "Number of working workshops: " << workshopsInWork << std::endl;
    std::cout << "Efficiency (%): " << efficiency << std::endl;
    std::cout << "Percentage of working workshops: " << getWorkshopPercentage() << "%" << std::endl << std::endl;
}

std::ostream& operator<<(std::ostream& out, const CompressorStation& station) {
    out << "CS" << std::endl;
    out << station.id << std::endl;
    out << station.name << std::endl;
    out << station.workshops << std::endl;
    out << station.workshopsInWork << std::endl;
    out << station.efficiency << std::endl;
    return out;
}

std::istream& operator>>(std::istream& in, CompressorStation& station) {
    std::string line;
    std::getline(in, line); // Read CS
    std::getline(in, line);
    station.id = std::stoi(line);
    std::getline(in, station.name);
    std::getline(in, line);
    station.workshops = std::stoi(line);
    std::getline(in, line);
    station.workshopsInWork = std::stoi(line);
    std::getline(in, line);
    station.efficiency = std::stod(line);
    return in;
}

std::ofstream& operator<<(std::ofstream& out, const CompressorStation& station) {
    out << "CS" << std::endl;
    out << station.id << std::endl;
    out << station.name << std::endl;
    out << station.workshops << std::endl;
    out << station.workshopsInWork << std::endl;
    out << station.efficiency << std::endl;
    return out;
}

std::ifstream& operator>>(std::ifstream& in, CompressorStation& station) {
    std::string line;
    std::getline(in, line);
    std::getline(in, line);
    station.id = std::stoi(line);
    std::getline(in, station.name);
    std::getline(in, line);
    station.workshops = std::stoi(line);
    std::getline(in, line);
    station.workshopsInWork = std::stoi(line);
    std::getline(in, line);
    station.efficiency = std::stod(line);
    return in;
}

void PipeManager::addPipe() {
    Pipe newPipe;
    newPipe.readFromConsole();
    pipes[newPipe.getId()] = newPipe;
    std::cout << "Pipe added with ID: " << newPipe.getId() << std::endl;
}

void PipeManager::displayAllPipes() const {
    if (pipes.empty()) {
        std::cout << "No pipes available." << std::endl;
        return;
    }
    std::cout << "=== ALL PIPES ===" << std::endl;
    for (const auto& pair : pipes) {
        pair.second.writeToConsole();
    }
}

void PipeManager::displayPipes(const std::unordered_map<int, Pipe>& pipeSubset) const {
    if (pipeSubset.empty()) {
        std::cout << "No pipes matching search criteria." << std::endl;
        return;
    }
    std::cout << "=== FOUND PIPES ===" << std::endl;
    for (const auto& pair : pipeSubset) {
        pair.second.writeToConsole();
    }
}

void PipeManager::editPipe() {
    if (pipes.empty()) {
        std::cout << "No pipes available for editing." << std::endl;
        return;
    }

    displayAllPipes();
    int id = inputInRange<int>("Enter pipe ID for editing: ", 1, Pipe::nextId - 1);

    auto it = pipes.find(id);
    if (it != pipes.end()) {
        it->second.editRepairStatus();
    }
    else {
        std::cout << "Pipe with ID " << id << " not found." << std::endl;
    }
}

void PipeManager::deletePipe() {
    if (pipes.empty()) {
        std::cout << "No pipes available for deletion." << std::endl;
        return;
    }

    displayAllPipes();
    int id = inputInRange<int>("Enter pipe ID for deletion: ", 1, Pipe::nextId - 1);

    auto it = pipes.find(id);
    if (it != pipes.end()) {
        pipes.erase(it);
        std::cout << "Pipe with ID " << id << " deleted." << std::endl;
    }
    else {
        std::cout << "Pipe with ID " << id << " not found." << std::endl;
    }
}

std::unordered_map<int, Pipe> PipeManager::searchByName(const std::string& name) const {
    std::unordered_map<int, Pipe> result;
    for (const auto& pair : pipes) {
        if (pair.second.getName().find(name) != std::string::npos) {
            result[pair.first] = pair.second;
        }
    }
    return result;
}

std::unordered_map<int, Pipe> PipeManager::searchByStatus(bool status) const {
    std::unordered_map<int, Pipe> result;
    for (const auto& pair : pipes) {
        if (pair.second.getStatus() == status) {
            result[pair.first] = pair.second;
        }
    }
    return result;
}

void PipeManager::editPipeSubset(const std::unordered_map<int, Pipe>& pipeSubset) {
    for (const auto& pair : pipeSubset) {
        auto pipeIt = pipes.find(pair.first);
        if (pipeIt != pipes.end()) {
            pipeIt->second.editRepairStatus();
        }
    }
}

void PipeManager::deletePipeSubset(const std::unordered_map<int, Pipe>& pipeSubset) {
    for (const auto& pair : pipeSubset) {
        pipes.erase(pair.first);
    }
}

// Реализация класса CSManager
void CSManager::addCS() {
    CompressorStation newCS;
    newCS.readFromConsole();
    stations[newCS.getId()] = newCS;
    std::cout << "Compressor station added with ID: " << newCS.getId() << std::endl;
}

void CSManager::displayAllCS() const {
    if (stations.empty()) {
        std::cout << "No compressor stations available." << std::endl;
        return;
    }
    std::cout << "=== ALL COMPRESSOR STATIONS ===" << std::endl;
    for (const auto& pair : stations) {
        pair.second.writeToConsole();
    }
}

void CSManager::displayCS(const std::unordered_map<int, CompressorStation>& csSubset) const {
    if (csSubset.empty()) {
        std::cout << "No compressor stations matching search criteria." << std::endl;
        return;
    }
    std::cout << "=== FOUND COMPRESSOR STATIONS ===" << std::endl;
    for (const auto& pair : csSubset) {
        pair.second.writeToConsole();
    }
}

void CSManager::editCS() {
    if (stations.empty()) {
        std::cout << "No stations available for editing." << std::endl;
        return;
    }

    displayAllCS();
    int id = inputInRange<int>("Enter station ID for editing: ", 1, CompressorStation::nextId - 1);

    auto it = stations.find(id);
    if (it != stations.end()) {
        it->second.editWorkshop();
    }
    else {
        std::cout << "Station with ID " << id << " not found." << std::endl;
    }
}

void CSManager::deleteCS() {
    if (stations.empty()) {
        std::cout << "No stations available for deletion." << std::endl;
        return;
    }

    displayAllCS();
    int id = inputInRange<int>("Enter station ID for deletion: ", 1, CompressorStation::nextId - 1);

    auto it = stations.find(id);
    if (it != stations.end()) {
        stations.erase(it);
        std::cout << "Station with ID " << id << " deleted." << std::endl;
    }
    else {
        std::cout << "Station with ID " << id << " not found." << std::endl;
    }
}

std::unordered_map<int, CompressorStation> CSManager::searchByName(const std::string& name) const {
    std::unordered_map<int, CompressorStation> result;
    for (const auto& pair : stations) {
        if (pair.second.getName().find(name) != std::string::npos) {
            result[pair.first] = pair.second;
        }
    }
    return result;
}

std::unordered_map<int, CompressorStation> CSManager::searchByWorkshopPercentage(double minPercent, double maxPercent) const {
    std::unordered_map<int, CompressorStation> result;
    for (const auto& pair : stations) {
        double percentage = pair.second.getWorkshopPercentage();
        if (percentage >= minPercent && percentage <= maxPercent) {
            result[pair.first] = pair.second;
        }
    }
    return result;
}

void CSManager::editCSSubset(const std::unordered_map<int, CompressorStation>& csSubset) {
    for (const auto& pair : csSubset) {
        auto csIt = stations.find(pair.first);
        if (csIt != stations.end()) {
            csIt->second.editWorkshop();
        }
    }
}

void CSManager::deleteCSSubset(const std::unordered_map<int, CompressorStation>& csSubset) {
    for (const auto& pair : csSubset) {
        stations.erase(pair.first);
    }
}

// Функции для работы с файлами
void saveToFile(const PipeManager& pipeManager, const CSManager& csManager) {
    std::ofstream outFile("DataPipeAndCs.txt");
    if (outFile.is_open()) {
        outFile << "COUNTERS" << std::endl;
        outFile << Pipe::nextId << std::endl;
        outFile << CompressorStation::nextId << std::endl;

        for (const auto& pair : pipeManager.getPipes()) {
            outFile << pair.second;
        }

        for (const auto& pair : csManager.getStations()) {
            outFile << pair.second;
        }

        outFile.close();
        std::cout << "Data saved to DataPipeAndCs.txt" << std::endl;
    }
    else {
        std::cout << "Failed to open file for writing" << std::endl;
    }
}

void loadFromFile(PipeManager& pipeManager, CSManager& csManager) {
    std::ifstream inFile("DataPipeAndCs.txt");
    if (inFile.is_open()) {
        std::string line;
        bool countersLoaded = false;

        while (std::getline(inFile, line)) {
            try {
                if (line == "COUNTERS" && !countersLoaded) {
                    std::getline(inFile, line);
                    Pipe::nextId = std::stoi(line);
                    std::getline(inFile, line);
                    CompressorStation::nextId = std::stoi(line);
                    countersLoaded = true;
                }
                else if (line == "PIPE") {
                    Pipe pipe;
                    inFile >> pipe;
                    pipeManager.getPipes()[pipe.getId()] = pipe;
                }
                else if (line == "CS") {
                    CompressorStation station;
                    inFile >> station;
                    csManager.getStations()[station.getId()] = station;
                }
            }
            catch (const std::exception& e) {
                std::cerr << "Error loading data: " << e.what() << std::endl;
                continue;
            }
        }
        inFile.close();
        std::cout << "Data loaded from DataPipeAndCs.txt" << std::endl;
    }
    else {
        std::cout << "Failed to open file for reading" << std::endl;
    }
}

// Функции для меню поиска
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