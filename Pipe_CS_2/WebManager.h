#ifndef WEBMANAGER_H
#define WEBMANAGER_H

#include "web.h"
#include "PipeManager.h"
#include "CSManager.h"
#include <vector>

class WebManager {
private:
    Web network;
    const std::vector<double> allowedDiameters = { 530, 720, 1020, 1220, 1420 };

public:
    void addConnection(PipeManager& pipeManager, CSManager& csManager);
    void removeConnection();
    void displayConnections() const;
    void topologicalSort() const;
    void checkCycles() const;

    // New methods for flow and path calculations
    void calculateMaxFlow(PipeManager& pipeManager, CSManager& csManager) const;
    void findShortestPath(PipeManager& pipeManager, CSManager& csManager) const;

    Web& getNetwork() { return network; }
    const Web& getNetwork() const { return network; }

private:
    void displayPipesByDiameter(const PipeManager& pipeManager) const;
    bool isDiameterAllowed(double diameter) const;
};

#endif