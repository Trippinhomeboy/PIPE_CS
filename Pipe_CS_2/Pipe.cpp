#include "Pipe.h"
#include "Utilities.h"
#include <iostream>
#include <string>

int Pipe::nextId = 1;

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
    std::getline(in, line); // Read "PIPE"
    std::getline(in, line);
    pipe.id = std::stoi(line);
    if (pipe.id >= Pipe::nextId) {
        Pipe::nextId = pipe.id + 1;
    }
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
    if (pipe.id >= Pipe::nextId) {
        Pipe::nextId = pipe.id + 1;
    }
    std::getline(in, pipe.name);
    std::getline(in, line);
    pipe.length = std::stod(line);
    std::getline(in, line);
    pipe.diameter = std::stod(line);
    std::getline(in, line);
    pipe.status = (line == "1");
    return in;
}