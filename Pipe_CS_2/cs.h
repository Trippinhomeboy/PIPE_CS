#ifndef CS_H
#define CS_H

#include <iostream>
#include <string>
#include <fstream>

class CompressorStation {
private:
    int id;
    std::string name; 
    int workshops;
    int workshopsInWork;
    double efficiency;

public:
    static int nextId;

    CompressorStation();
    CompressorStation(const std::string& name, int workshops, int workshopsInWork, double efficiency);

    int getId() const { return id; }
    std::string getName() const { return name; }
    int getWorkshops() const { return workshops; }
    int getWorkshopsInWork() const { return workshopsInWork; }
    double getEfficiency() const { return efficiency; }

    void setName(const std::string& newName) { name = newName; }
    void setWorkshops(int newWorkshops) { workshops = newWorkshops; }
    void setWorkshopsInWork(int newWorkshopsInWork) { workshopsInWork = newWorkshopsInWork; }
    void setEfficiency(double newEfficiency) { efficiency = newEfficiency; }

    double getWorkshopPercentage() const { return (workshops > 0) ? (workshopsInWork * 100.0 / workshops) : 0; }

    void editWorkshop();
    void readFromConsole();
    void writeToConsole() const;

    friend std::ostream& operator<<(std::ostream& out, const CompressorStation& station);
    friend std::istream& operator>>(std::istream& in, CompressorStation& station);
    friend std::ofstream& operator<<(std::ofstream& out, const CompressorStation& station);
    friend std::ifstream& operator>>(std::ifstream& in, CompressorStation& station);
};

#endif