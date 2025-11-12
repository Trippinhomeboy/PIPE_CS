#ifndef WEBMANAGER_H
#define WEBMANAGER_H

#include "web.h"
#include "PipeManager.h"
#include "CSManager.h"
#include <vector>

class WebManager {
private:
    Web network;
    const std::vector<double> allowedDiameters = { 500, 700, 1000, 1400 };

public:
    void addConnection(PipeManager& pipeManager, CSManager& csManager);
    void removeConnection();
    void displayConnections() const;
    void topologicalSort() const;
    void checkCycles() const;

    Web& getNetwork() { return network; }
    const Web& getNetwork() const { return network; }

private:
    void displayPipesByDiameter(const PipeManager& pipeManager) const;
    bool isDiameterAllowed(double diameter) const;
};

#endif