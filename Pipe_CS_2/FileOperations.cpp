#include "FileOperations.h"
#include "Utilities.h"
#include <iostream>
#include <string>
#include <fstream>

void saveToFile(const PipeManager& pipeManager, const CSManager& csManager) {
    std::cout << "Enter filename for saving: ";
    std::string filename;
    std::getline(std::cin, filename);

    if (filename.empty()) {
        filename = "DataPipeAndCs.txt";
    }

    std::ofstream outFile(filename);
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
        std::cout << "Data saved to " << filename << std::endl;
    }
    else {
        std::cout << "Failed to open file " << filename << " for writing" << std::endl;
    }
}

void loadFromFile(PipeManager& pipeManager, CSManager& csManager) {
    std::cout << "Enter filename for loading: ";
    std::string filename;
    std::getline(std::cin, filename);

    if (filename.empty()) {
        filename = "DataPipeAndCs.txt";
    }

    std::ifstream inFile(filename);
    if (inFile.is_open()) {
        std::string line;
        bool countersLoaded = false;

        // Clear existing data
        pipeManager.getPipes().clear();
        csManager.getStations().clear();

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
        std::cout << "Data loaded from " << filename << std::endl;
    }
    else {
        std::cout << "Failed to open file " << filename << " for reading" << std::endl;
    }
}