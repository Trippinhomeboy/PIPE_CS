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

    bool hasAllowedPipes = false;
    for (const auto& pipePair : pipeManager.getPipes()) {
        if (isDiameterAllowed(pipePair.second.getDiameter())) {
            hasAllowedPipes = true;
            break;
        }
    }

    if (!hasAllowedPipes) {
        std::cout << "No pipes with allowed diameters (530, 720, 1020, 1220, 1420 mm) available." << std::endl;
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

    displayPipesByDiameter(pipeManager);

    std::cout << "Allowed diameters: ";
    for (size_t i = 0; i < allowedDiameters.size(); ++i) {
        std::cout << allowedDiameters[i] << "mm";
        if (i != allowedDiameters.size() - 1) {
            std::cout << ", ";
        }
    }
    std::cout << std::endl;

    // Let user choose from allowed diameters only
    std::cout << "Choose diameter:" << std::endl;
    for (size_t i = 0; i < allowedDiameters.size(); ++i) {
        std::cout << (i + 1) << ") " << allowedDiameters[i] << "mm" << std::endl;
    }

    int diameterChoice = inputInRange<int>("Enter your choice: ", 1, static_cast<int>(allowedDiameters.size()));
    double chosenDiameter = allowedDiameters[diameterChoice - 1];

    std::vector<int> availablePipeIds;
    for (const auto& pipePair : pipeManager.getPipes()) {
        const Pipe& pipe = pipePair.second;
        if (pipe.getDiameter() == chosenDiameter) {
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

    int pipeId;
    if (availablePipeIds.size() == 1) {
        pipeId = availablePipeIds[0];
        std::cout << "Using pipe ID " << pipeId << " with diameter " << chosenDiameter << "mm." << std::endl;
    }
    else {
        std::cout << "Available pipes with diameter " << chosenDiameter << "mm:" << std::endl;
        for (int id : availablePipeIds) {
            const Pipe& pipe = pipeManager.getPipes().at(id);
            std::cout << "ID: " << pipe.getId() << " | Name: " << pipe.getName()
                << " | Length: " << pipe.getLength() << "km" << std::endl;
        }

        int minId = *std::min_element(availablePipeIds.begin(), availablePipeIds.end());
        int maxId = *std::max_element(availablePipeIds.begin(), availablePipeIds.end());
        pipeId = inputInRange<int>("Enter pipe ID to use: ", minId, maxId);

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

void WebManager::calculateMaxFlow(PipeManager& pipeManager, CSManager& csManager) const {
    if (network.getConnections().empty()) {
        std::cout << "No connections in the network." << std::endl;
        return;
    }

    csManager.displayAllCS();
    int source = inputInRange<int>("Enter source CS ID: ", 1, CompressorStation::maxId);
    int sink = inputInRange<int>("Enter sink CS ID: ", 1, CompressorStation::maxId);

    if (csManager.getStations().find(source) == csManager.getStations().end()) {
        std::cout << "Error: Source CS with ID " << source << " not found." << std::endl;
        return;
    }

    if (csManager.getStations().find(sink) == csManager.getStations().end()) {
        std::cout << "Error: Sink CS with ID " << sink << " not found." << std::endl;
        return;
    }

    if (source == sink) {
        std::cout << "Error: Source and sink cannot be the same." << std::endl;
        return;
    }

    double maxFlow = network.findMaxFlow(source, sink, pipeManager.getPipes());
    std::cout << "Maximum flow from CS" << source << " to CS" << sink << ": "
        << maxFlow << " million m³ per day" << std::endl;
}

void WebManager::findShortestPath(PipeManager& pipeManager, CSManager& csManager) const {
    if (network.getConnections().empty()) {
        std::cout << "No connections in the network." << std::endl;
        return;
    }

    csManager.displayAllCS();
    int startCSId = inputInRange<int>("Enter start CS ID: ", 1, CompressorStation::maxId);
    int endCSId = inputInRange<int>("Enter end CS ID: ", 1, CompressorStation::maxId);

    if (csManager.getStations().find(startCSId) == csManager.getStations().end()) {
        std::cout << "Error: Start CS with ID " << startCSId << " not found." << std::endl;
        return;
    }

    if (csManager.getStations().find(endCSId) == csManager.getStations().end()) {
        std::cout << "Error: End CS with ID " << endCSId << " not found." << std::endl;
        return;
    }

    if (startCSId == endCSId) {
        std::cout << "Start and end CS are the same." << std::endl;
        return;
    }

    std::vector<int> path = network.findShortestPath(startCSId, endCSId, pipeManager.getPipes());

    if (path.empty()) {
        std::cout << "No path found from CS" << startCSId << " to CS" << endCSId << std::endl;
    }
    else {
        std::cout << "Shortest path from CS" << startCSId << " to CS" << endCSId << ": ";
        for (size_t i = 0; i < path.size(); ++i) {
            std::cout << "CS" << path[i];
            if (i != path.size() - 1) {
                std::cout << " -> ";
            }
        }
        std::cout << std::endl;

        // Calculate total distance
        double totalDistance = 0.0;
        for (size_t i = 0; i < path.size() - 1; ++i) {
            int from = path[i];
            int to = path[i + 1];

            // Find the pipe for this connection
            for (const auto& conn : network.getConnections()) {
                if (conn.startCSId == from && conn.endCSId == to) {
                    auto pipeIt = pipeManager.getPipes().find(conn.pipeId);
                    if (pipeIt != pipeManager.getPipes().end()) {
                        totalDistance += pipeIt->second.getLength();
                    }
                    break;
                }
            }
        }
        std::cout << "Total distance: " << totalDistance << " km" << std::endl;
    }
}

void WebManager::displayPipesByDiameter(const PipeManager& pipeManager) const {
    const auto& pipes = pipeManager.getPipes();
    if (pipes.empty()) {
        std::cout << "No pipes available." << std::endl;
        return;
    }

    std::cout << "=== PIPES GROUPED BY DIAMETER ===" << std::endl;

    std::map<double, std::vector<const Pipe*>> pipesByDiameter;
    for (const auto& pipePair : pipes) {
        const Pipe& pipe = pipePair.second;
        pipesByDiameter[pipe.getDiameter()].push_back(&pipe);
    }

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