#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <vector>

class Pipe {
private:
    int id;
    std::string name;
    double length;
    double diameter;
    bool status;

public:
    static int maxId;
    static const std::vector<double> allowedDiameters;

    Pipe();
    Pipe(const std::string& name, double length, double diameter, bool status = false);

    int getId() const { return id; }
    std::string getName() const { return name; }
    double getLength() const { return length; }
    double getDiameter() const { return diameter; }
    bool getStatus() const { return status; }

    void setName(const std::string& newName) { name = newName; }
    void setLength(double newLength) { length = newLength; }
    void setDiameter(double newDiameter) { diameter = newDiameter; }
    void setStatus(bool newStatus) { status = newStatus; }

    void editRepairStatus();
    void readFromConsole();
    void writeToConsole() const;


    void saveToFile(std::ofstream& out) const;
    void loadFromFile(std::ifstream& in);

    friend std::ostream& operator<<(std::ostream& out, const Pipe& pipe);
    friend std::istream& operator>>(std::istream& in, Pipe& pipe);
    friend std::ofstream& operator<<(std::ofstream& out, const Pipe& pipe);
    friend std::ifstream& operator>>(std::ifstream& in, Pipe& pipe);
};