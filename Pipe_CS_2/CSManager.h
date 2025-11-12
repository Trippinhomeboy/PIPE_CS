#ifndef CSMANAGER_H
#define CSMANAGER_H

#include "cs.h"
#include "web.h"
#include <unordered_map>

class CSManager {
private:
    std::unordered_map<int, CompressorStation> stations;

public:
    void addCS();
    void displayAllCS() const;
    void displayCS(const std::unordered_map<int, CompressorStation>& csSubset) const;
    void editCS();
    void deleteCS(Web& network); // Updated to accept network reference

    std::unordered_map<int, CompressorStation> searchByName(const std::string& name) const;
    std::unordered_map<int, CompressorStation> searchByWorkshopPercentage(double minPercent, double maxPercent) const;

    void editCSSubset(const std::unordered_map<int, CompressorStation>& csSubset);
    void deleteCSSubset(const std::unordered_map<int, CompressorStation>& csSubset, Web& network); // Updated

    const std::unordered_map<int, CompressorStation>& getStations() const { return stations; }
    std::unordered_map<int, CompressorStation>& getStations() { return stations; }
};

#endif