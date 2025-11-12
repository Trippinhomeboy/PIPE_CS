#include "WebManager.h"
#include "Utilities.h"
#include <iostream>
#include <iomanip>
#include <map>
#include <algorithm>

void WebManager::addConnection(PipeManager& pipeManager, CSManager& csManager) {
    if (csManager.getStations().size() < 2) {
        std::cout << "Need at least 2 compressor stations to create connection." << std::endl;
        return;
    }

    // Check if there are pipes with allowed diameters
    bool hasAllowedPipes = false;
    for (const auto& pipePair : pipeManager.getPipes()) {
        if (isDiameterAllowed(pipePair.second.getDiameter())) {
            hasAllowedPipes = true;
            break;
        }
    }

    if (!hasAllowedPipes) {
        std::cout << "No pipes with allowed diameters (500, 700, 1000, 1400 mm) available." << std::endl;
        std::cout << "You need to create a pipe with allowed diameter first." << std::endl;
        return;
    }

    csManager.displayAllCS();
    int startCSId = inputInRange<int>("Enter start CS ID: ", 1, CompressorStation::maxId);
    int endCSId = inputInRange<int>("Enter end CS ID: ", 1, CompressorStation::maxId);

    if (startCSId == endCSId) {
        std::cout << "Error: Cannot create connection from CS to itself." << std::endl;
        return;
    }

    // Check if CS exist
    if (csManager.getStations().find(startCSId) == csManager.getStations().end()) {
        std::cout << "Error: Start CS with ID " << startCSId << " not found." << std::endl;
        return;
    }

    if (csManager.getStations().find(endCSId) == csManager.getStations().end()) {
        std::cout << "Error: End CS with ID " << endCSId << " not found." << std::endl;
        return;
    }

    if (network.connectionExists(startCSId, endCSId)) {
        std::cout << "Error: Connection already exists." << std::endl;
        return;
    }

    if (network.hasReverseConnection(startCSId, endCSId)) {
        std::cout << "Warning: Reverse connection exists." << std::endl;
    }

    // Display pipes grouped by diameter
    displayPipesByDiameter(pipeManager);

    // Ask for diameter choice
    std::cout << "Allowed diameters: ";
    for (size_t i = 0; i < allowedDiameters.size(); ++i) {
        std::cout << allowedDiameters[i] << "mm";
        if (i != allowedDiameters.size() - 1) {
            std::cout << ", ";
        }
    }
    std::cout << std::endl;

    double chosenDiameter = inputInRange<double>("Enter pipe diameter to use: ", 500.0, 1400.0);

    // Check if diameter is allowed
    if (!isDiameterAllowed(chosenDiameter)) {
        std::cout << "Error: Diameter " << chosenDiameter << "mm is not allowed. Use 500, 700, 1000 or 1400mm." << std::endl;
        return;
    }

    // Find available pipes with chosen diameter
    std::vector<int> availablePipeIds;
    for (const auto& pipePair : pipeManager.getPipes()) {
        const Pipe& pipe = pipePair.second;
        if (pipe.getDiameter() == chosenDiameter) {
            // Check if pipe is not already used in connections
            bool pipeUsed = false;
            for (const auto& conn : network.getConnections()) {
                if (conn.pipeId == pipe.getId()) {
                    pipeUsed = true;
                    break;
                }
            }
            if (!pipeUsed) {
                availablePipeIds.push_back(pipe.getId());
            }
        }
    }

    if (availablePipeIds.empty()) {
        std::cout << "Error: No available pipes with diameter " << chosenDiameter << "mm." << std::endl;
        std::cout << "All pipes with this diameter are already used in connections or don't exist." << std::endl;
        return;
    }

    // If only one pipe available, use it automatically
    int pipeId;
    if (availablePipeIds.size() == 1) {
        pipeId = availablePipeIds[0];
        std::cout << "Using pipe ID " << pipeId << " with diameter " << chosenDiameter << "mm." << std::endl;
    }
    else {
        // Display available pipes
        std::cout << "Available pipes with diameter " << chosenDiameter << "mm:" << std::endl;
        for (int id : availablePipeIds) {
            const Pipe& pipe = pipeManager.getPipes().at(id);
            std::cout << "ID: " << pipe.getId() << " | Name: " << pipe.getName()
                << " | Length: " << pipe.getLength() << "km" << std::endl;
        }

        // Get min and max ID from available pipes
        int minId = *std::min_element(availablePipeIds.begin(), availablePipeIds.end());
        int maxId = *std::max_element(availablePipeIds.begin(), availablePipeIds.end());
        pipeId = inputInRange<int>("Enter pipe ID to use: ", minId, maxId);

        // Check if chosen ID is in available list
        if (std::find(availablePipeIds.begin(), availablePipeIds.end(), pipeId) == availablePipeIds.end()) {
            std::cout << "Error: Pipe ID " << pipeId << " is not available or has wrong diameter." << std::endl;
            return;
        }
    }

    network.addConnection(startCSId, endCSId, pipeId);
    std::cout << "Connection created successfully: CS" << startCSId << " -> CS" << endCSId
        << " (Pipe ID: " << pipeId << ", Diameter: " << chosenDiameter << "mm)" << std::endl;
}

void WebManager::removeConnection() {
    if (network.getConnections().empty()) {
        std::cout << "No connections available." << std::endl;
        return;
    }

    displayConnections();
    int startCSId = inputInRange<int>("Enter start CS ID to remove connection: ", 1, CompressorStation::maxId);
    int endCSId = inputInRange<int>("Enter end CS ID to remove connection: ", 1, CompressorStation::maxId);

    if (network.removeConnection(startCSId, endCSId)) {
        std::cout << "Connection removed successfully." << std::endl;
    }
    else {
        std::cout << "Connection not found." << std::endl;
    }
}

void WebManager::displayConnections() const {
    const auto& connections = network.getConnections();
    if (connections.empty()) {
        std::cout << "No connections in the network." << std::endl;
        return;
    }

    std::cout << "=== NETWORK CONNECTIONS ===" << std::endl;
    for (const auto& conn : connections) {
        std::cout << "CS" << conn.startCSId << " -> CS" << conn.endCSId << " (Pipe ID: " << conn.pipeId << ")" << std::endl;
    }
}

void WebManager::topologicalSort() const {
    if (network.getConnections().empty()) {
        std::cout << "No connections in the network." << std::endl;
        return;
    }

    std::vector<int> sorted = network.topologicalSort();
    if (sorted.empty()) {
        std::cout << "Network contains cycles. Topological sort not possible." << std::endl;
    }
    else {
        std::cout << "Topological sort order: ";
        for (size_t i = 0; i < sorted.size(); ++i) {
            std::cout << "CS" << sorted[i];
            if (i != sorted.size() - 1) {
                std::cout << " -> ";
            }
        }
        std::cout << std::endl;
    }
}

void WebManager::checkCycles() const {
    if (network.hasCycle()) {
        std::cout << "Network contains cycles." << std::endl;
    }
    else {
        std::cout << "Network is acyclic." << std::endl;
    }
}

void WebManager::displayPipesByDiameter(const PipeManager& pipeManager) const {
    const auto& pipes = pipeManager.getPipes();
    if (pipes.empty()) {
        std::cout << "No pipes available." << std::endl;
        return;
    }

    std::cout << "=== PIPES GROUPED BY DIAMETER ===" << std::endl;

    // Group pipes by diameter
    std::map<double, std::vector<const Pipe*>> pipesByDiameter;
    for (const auto& pipePair : pipes) {
        const Pipe& pipe = pipePair.second;
        pipesByDiameter[pipe.getDiameter()].push_back(&pipe);
    }

    // Display pipes for each diameter
    for (const auto& diameterGroup : pipesByDiameter) {
        double diameter = diameterGroup.first;
        const auto& pipeList = diameterGroup.second;

        std::cout << "Diameter: " << diameter << "mm ";
        if (isDiameterAllowed(diameter)) {
            std::cout << "(ALLOWED)";
        }
        else {
            std::cout << "(NOT ALLOWED)";
        }
        std::cout << std::endl;

        for (const Pipe* pipe : pipeList) {
            // Check if pipe is used in connections
            bool isUsed = false;
            for (const auto& conn : network.getConnections()) {
                if (conn.pipeId == pipe->getId()) {
                    isUsed = true;
                    break;
                }
            }

            std::cout << "  ID: " << std::setw(4) << pipe->getId()
                << " | Name: " << std::setw(15) << std::left << pipe->getName()
                << " | Length: " << std::setw(8) << pipe->getLength() << "km"
                << " | Status: " << (pipe->getStatus() ? "In repair" : "Working")
                << " | " << (isUsed ? "USED" : "AVAILABLE") << std::endl;
        }
        std::cout << std::endl;
    }
}

bool WebManager::isDiameterAllowed(double diameter) const {
    return std::find(allowedDiameters.begin(), allowedDiameters.end(), diameter) != allowedDiameters.end();
}