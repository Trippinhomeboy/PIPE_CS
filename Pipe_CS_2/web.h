#pragma once
#include "Pipe.h"
#include "cs.h"
#include <unordered_map>
#include <vector>
#include <set>
#include <algorithm>
#include <queue>
#include <limits>
#include <functional>

struct Connection {
    int startCSId;
    int endCSId;
    int pipeId;

    Connection(int start, int end, int pipe) : startCSId(start), endCSId(end), pipeId(pipe) {}

    bool operator==(const Connection& other) const {
        bool startEqual = (startCSId == other.startCSId);
        bool endEqual = (endCSId == other.endCSId);
        bool pipeEqual = (pipeId == other.pipeId);
        return startEqual && endEqual && pipeEqual;
    }
};

class Web {
private:
    std::vector<Connection> connections;
    std::unordered_map<int, std::vector<int>> adjacencyList;

public:
    void addConnection(int startCSId, int endCSId, int pipeId);
    bool removeConnection(int startCSId, int endCSId);
    void removeConnectionsWithPipe(int pipeId);
    void removeConnectionsWithCS(int csId);

    bool connectionExists(int startCSId, int endCSId) const;
    bool hasReverseConnection(int startCSId, int endCSId) const;
    bool hasSelfConnection(int csId) const;

    const std::vector<Connection>& getConnections() const { return connections; }
    const std::unordered_map<int, std::vector<int>>& getAdjacencyList() const { return adjacencyList; }

    std::vector<int> topologicalSort() const;
    bool hasCycle() const;

    // New methods for flow and path calculations
    double calculatePipeCapacity(const Pipe& pipe) const;
    double calculatePipeWeight(const Pipe& pipe) const;
    double findMaxFlow(int source, int sink, const std::unordered_map<int, Pipe>& pipes) const;
    std::vector<int> findShortestPath(int startCSId, int endCSId, const std::unordered_map<int, Pipe>& pipes) const;

private:
    bool topologicalSortUtil(int v, std::unordered_map<int, bool>& visited,
        std::unordered_map<int, bool>& recStack,
        std::vector<int>& result) const;
};