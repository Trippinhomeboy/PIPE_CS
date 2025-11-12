#pragma once
#include "Pipe.h"
#include "cs.h"
#include <unordered_map>
#include <vector>
#include <set>
#include <algorithm>

struct Connection {
    int startCSId;
    int endCSId;
    int pipeId;

    Connection(int start, int end, int pipe) : startCSId(start), endCSId(end), pipeId(pipe) {}

    bool operator==(const Connection& other) const {
        return startCSId == other.startCSId && endCSId == other.endCSId && pipeId == other.pipeId;
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

private:
    bool topologicalSortUtil(int v, std::unordered_map<int, bool>& visited,
        std::unordered_map<int, bool>& recStack,
        std::vector<int>& result) const;
};