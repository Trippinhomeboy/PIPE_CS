#include "CSManager.h"
#include "Utilities.h"
#include <iostream>
#include <string>

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
    int id = inputInRange<int>("Enter station ID for editing: ", 1, CompressorStation::maxId);

    auto it = stations.find(id);
    if (it != stations.end()) {
        it->second.editWorkshop();
    }
    else {
        std::cout << "Station with ID " << id << " not found." << std::endl;
    }
}

void CSManager::deleteCS(Web& network) {
    if (stations.empty()) {
        std::cout << "No stations available for deletion." << std::endl;
        return;
    }

    displayAllCS();
    int id = inputInRange<int>("Enter station ID for deletion: ", 1, CompressorStation::maxId);

    auto it = stations.find(id);
    if (it != stations.end()) {
        // Remove connections that involve this CS
        network.removeConnectionsWithCS(id);
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

void CSManager::deleteCSSubset(const std::unordered_map<int, CompressorStation>& csSubset, Web& network) {
    for (const auto& pair : csSubset) {
        // Remove connections that involve this CS
        network.removeConnectionsWithCS(pair.first);
        stations.erase(pair.first);
    }
}