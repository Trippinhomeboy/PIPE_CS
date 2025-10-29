#include "cs.h"
#include "Utilities.h"
#include <iostream>
#include <string>

int CompressorStation::maxId = 0;

CompressorStation::CompressorStation() : id(0), name(""), workshops(0), workshopsInWork(0), efficiency(0) {
}

CompressorStation::CompressorStation(const std::string& name, int workshops, int workshopsInWork, double efficiency)
    : id(0), name(name), workshops(workshops), workshopsInWork(workshopsInWork), efficiency(efficiency) {
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

    // Recalculate efficiency after editing workshops
    efficiency = getWorkshopPercentage();
}

void CompressorStation::readFromConsole() {
    std::cout << "Enter station name: ";
    std::getline(std::cin, name);

    workshops = inputInRange<int>("Enter number of workshops: ", 1, 10000);
    workshopsInWork = inputInRange<int>("Enter number of working workshops: ", 0, workshops);
    // Efficiency is calculated automatically based on workshop percentage
    efficiency = getWorkshopPercentage();

    // Assign ID only when creating new object from console
    if (id == 0) {
        id = ++maxId;
    }
}

void CompressorStation::writeToConsole() const {
    std::cout << "Station ID: " << id << std::endl;
    std::cout << "Station name: " << name << std::endl;
    std::cout << "Number of workshops: " << workshops << std::endl;
    std::cout << "Number of working workshops: " << workshopsInWork << std::endl;
    std::cout << "Efficiency (%): " << efficiency << std::endl << std::endl;
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
    std::getline(in, line); // Read "CS"
    std::getline(in, line);
    station.id = std::stoi(line);
    if (station.id > CompressorStation::maxId) {
        CompressorStation::maxId = station.id;
    }
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
    std::getline(in, line); // Read "CS"
    std::getline(in, line);
    station.id = std::stoi(line);
    if (station.id > CompressorStation::maxId) {
        CompressorStation::maxId = station.id;
    }
    std::getline(in, station.name);
    std::getline(in, line);
    station.workshops = std::stoi(line);
    std::getline(in, line);
    station.workshopsInWork = std::stoi(line);
    std::getline(in, line);
    station.efficiency = std::stod(line);
    return in;
}