#include "Pipe.h"
#include "Utilities.h"
#include <iostream>
#include <string>

int Pipe::maxId = 0;

Pipe::Pipe() : id(0), name(""), length(0), diameter(0), status(false) {
}

Pipe::Pipe(const std::string& name, double length, double diameter, bool status)
    : id(0), name(name), length(length), diameter(diameter), status(status) {
}

void Pipe::editRepairStatus() {
    status = !status;
    std::cout << "Repair status changed to: " << (status ? "Yes" : "No") << std::endl;
}

void Pipe::readFromConsole() {
    std::cout << "Enter pipe name: ";
    std::getline(std::cin, name);

    length = inputInRange<double>("Enter pipe length (in km): ", 1.0, 10000.0);

    
    std::cout << "Allowed diameters for network connections: 500, 700, 1000, 1400 mm" << std::endl;
    diameter = inputInRange<double>("Enter pipe diameter (in mm): ", 1.0, 10000.0);

    
    if (diameter != 500 && diameter != 700 && diameter != 1000 && diameter != 1400) {
        std::cout << "Warning: Diameter " << diameter << "mm is not standard." << std::endl;
        std::cout << "This pipe cannot be used in network connections." << std::endl;
    }

    status = false;

    
    if (id == 0) {
        id = ++maxId;
    }
}

void Pipe::writeToConsole() const {
    std::cout << std::endl;
    std::cout << "Pipe ID: " << id << std::endl;
    std::cout << "Pipe name: " << name << std::endl;
    std::cout << "Length (km): " << length << std::endl;
    std::cout << "Diameter (mm): " << diameter;
    if (diameter == 500 || diameter == 700 || diameter == 1000 || diameter == 1400) {
        std::cout << " (Standard - can be used in network)";
    }
    else {
        std::cout << " (Non-standard - cannot be used in network)";
    }
    std::cout << std::endl;
    std::cout << "Repair status: " << (status ? "Yes" : "No") << std::endl << std::endl;
}

void Pipe::saveToFile(std::ofstream& out) const {
    out << "PIPE" << std::endl;
    out << id << std::endl;
    out << name << std::endl;
    out << length << std::endl;
    out << diameter << std::endl;
    out << (status ? "1" : "0") << std::endl;
}

void Pipe::loadFromFile(std::ifstream& in) {
    std::string line;
    std::getline(in, line); // Skip "PIPE"
    std::getline(in, line);
    if (!line.empty()) id = std::stoi(line);
    if (id > Pipe::maxId) {
        Pipe::maxId = id;
    }
    std::getline(in, name);
    std::getline(in, line);
    if (!line.empty()) length = std::stod(line);
    std::getline(in, line);
    if (!line.empty()) diameter = std::stod(line);
    std::getline(in, line);
    status = (line == "1");
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
    std::getline(in, line); // Skip "PIPE"
    std::getline(in, line); // ID
    if (!line.empty()) pipe.id = std::stoi(line);
    if (pipe.id > Pipe::maxId) {
        Pipe::maxId = pipe.id;
    }
    std::getline(in, pipe.name);
    std::getline(in, line);
    if (!line.empty()) pipe.length = std::stod(line);
    std::getline(in, line);
    if (!line.empty()) pipe.diameter = std::stod(line);
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
    std::getline(in, line); // Skip "PIPE"
    std::getline(in, line); // ID
    if (!line.empty()) pipe.id = std::stoi(line);
    if (pipe.id > Pipe::maxId) {
        Pipe::maxId = pipe.id;
    }
    std::getline(in, pipe.name);
    std::getline(in, line);
    if (!line.empty()) pipe.length = std::stod(line);
    std::getline(in, line);
    if (!line.empty()) pipe.diameter = std::stod(line);
    std::getline(in, line);
    pipe.status = (line == "1");
    return in;
}