#include "web.h"
#include <iostream>
#include <algorithm>

void Web::addConnection(int startCSId, int endCSId, int pipeId) {
    connections.emplace_back(startCSId, endCSId, pipeId);
    adjacencyList[startCSId].push_back(endCSId);
}

bool Web::removeConnection(int startCSId, int endCSId) {
    auto it = std::find_if(connections.begin(), connections.end(),
        [startCSId, endCSId](const Connection& conn) {
            return conn.startCSId == startCSId && conn.endCSId == endCSId;
        });

    if (it != connections.end()) {
        connections.erase(it);

        // Remove from adjacency list
        auto adjIt = adjacencyList.find(startCSId);
        if (adjIt != adjacencyList.end()) {
            auto& neighbors = adjIt->second;
            neighbors.erase(std::remove(neighbors.begin(), neighbors.end(), endCSId), neighbors.end());
            if (neighbors.empty()) {
                adjacencyList.erase(adjIt);
            }
        }
        return true;
    }
    return false;
}

void Web::removeConnectionsWithPipe(int pipeId) {
    connections.erase(std::remove_if(connections.begin(), connections.end(),
        [pipeId](const Connection& conn) {
            return conn.pipeId == pipeId;
        }), connections.end());

    // Rebuild adjacency list
    adjacencyList.clear();
    for (const auto& conn : connections) {
        adjacencyList[conn.startCSId].push_back(conn.endCSId);
    }
}

void Web::removeConnectionsWithCS(int csId) {
    connections.erase(std::remove_if(connections.begin(), connections.end(),
        [csId](const Connection& conn) {
            return conn.startCSId == csId || conn.endCSId == csId;
        }), connections.end());

    // Rebuild adjacency list
    adjacencyList.clear();
    for (const auto& conn : connections) {
        adjacencyList[conn.startCSId].push_back(conn.endCSId);
    }
}

bool Web::connectionExists(int startCSId, int endCSId) const {
    return std::any_of(connections.begin(), connections.end(),
        [startCSId, endCSId](const Connection& conn) {
            return conn.startCSId == startCSId && conn.endCSId == endCSId;
        });
}

bool Web::hasReverseConnection(int startCSId, int endCSId) const {
    return connectionExists(endCSId, startCSId);
}

bool Web::hasSelfConnection(int csId) const {
    return connectionExists(csId, csId);
}

std::vector<int> Web::topologicalSort() const {
    std::vector<int> result;
    std::unordered_map<int, bool> visited;
    std::unordered_map<int, bool> recStack;

    // Initialize visited and recStack for all CS in adjacency list
    for (const auto& pair : adjacencyList) {
        visited[pair.first] = false;
        recStack[pair.first] = false;
        for (int neighbor : pair.second) {
            visited[neighbor] = false;
            recStack[neighbor] = false;
        }
    }

    for (const auto& pair : adjacencyList) {
        int node = pair.first;
        if (!visited[node]) {
            if (!topologicalSortUtil(node, visited, recStack, result)) {
                // Cycle detected
                return std::vector<int>();
            }
        }
    }

    std::reverse(result.begin(), result.end());
    return result;
}

bool Web::hasCycle() const {
    std::unordered_map<int, bool> visited;
    std::unordered_map<int, bool> recStack;

    // Initialize visited and recStack for all CS in adjacency list
    for (const auto& pair : adjacencyList) {
        visited[pair.first] = false;
        recStack[pair.first] = false;
        for (int neighbor : pair.second) {
            visited[neighbor] = false;
            recStack[neighbor] = false;
        }
    }

    for (const auto& pair : adjacencyList) {
        int node = pair.first;
        if (!visited[node]) {
            std::vector<int> dummy; // Создаем временный вектор для вызова
            if (!topologicalSortUtil(node, visited, recStack, dummy)) {
                return true;
            }
        }
    }
    return false;
}

bool Web::topologicalSortUtil(int v, std::unordered_map<int, bool>& visited,
    std::unordered_map<int, bool>& recStack,
    std::vector<int>& result) const {
    if (!visited[v]) {
        visited[v] = true;
        recStack[v] = true;

        auto it = adjacencyList.find(v);
        if (it != adjacencyList.end()) {
            for (int neighbor : it->second) {
                if (!visited[neighbor]) {
                    if (!topologicalSortUtil(neighbor, visited, recStack, result)) {
                        return false;
                    }
                }
                else if (recStack[neighbor]) {
                    return false; // Cycle detected
                }
            }
        }

        recStack[v] = false;
        result.push_back(v);
    }
    return true;
}