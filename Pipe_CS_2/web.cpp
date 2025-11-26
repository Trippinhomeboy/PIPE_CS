#include "web.h"
#include <iostream>
#include <algorithm>
#include <queue>
#include <limits>
#include <functional>

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
            std::vector<int> dummy;
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
                    return false;
                }
            }
        }

        recStack[v] = false;
        result.push_back(v);
    }
    return true;
}

double Web::calculatePipeCapacity(const Pipe& pipe) const {
    if (pipe.getStatus()) {
        return 0.0; 
    }

   
    std::unordered_map<double, std::pair<double, double>> capacityData = {
        {1420, {90.0, 100.0}},
        {1220, {50.0, 60.0}},
        {1020, {25.0, 35.0}},
        {720, {11.0, 13.0}},
        {530, {4.0, 6.0}}
    };

    double diameter = pipe.getDiameter();
    auto it = capacityData.find(diameter);
    if (it != capacityData.end()) {
        
        return (it->second.first + it->second.second) / 2.0;
    }

    return 0.0; 
}

double Web::calculatePipeWeight(const Pipe& pipe) const {
    if (pipe.getStatus()) {
        return std::numeric_limits<double>::infinity(); 
    }

    
    return pipe.getLength();
}

double Web::findMaxFlow(int source, int sink, const std::unordered_map<int, Pipe>& pipes) const {
    
    std::unordered_map<int, std::unordered_map<int, double>> residual;

    
    for (const auto& conn : connections) {
        double capacity = 0.0;
        auto pipeIt = pipes.find(conn.pipeId);
        if (pipeIt != pipes.end()) {
            capacity = calculatePipeCapacity(pipeIt->second);
        }
        residual[conn.startCSId][conn.endCSId] = capacity;
       
        residual[conn.endCSId][conn.startCSId] = 0.0;
    }

    double maxFlow = 0.0;

    
    while (true) {
        
        std::unordered_map<int, int> parent;
        std::queue<int> q;
        q.push(source);
        parent[source] = -1;

        bool foundPath = false;
        while (!q.empty() && !foundPath) {
            int u = q.front();
            q.pop();

            for (const auto& neighbor : residual[u]) {
                int v = neighbor.first;
                double capacity = neighbor.second;

                if (parent.find(v) == parent.end() && capacity > 1e-9) {
                    parent[v] = u;
                    if (v == sink) {
                        foundPath = true;
                        break;
                    }
                    q.push(v);
                }
            }
        }

        if (!foundPath) {
            break; 
        }

        
        double pathFlow = std::numeric_limits<double>::infinity();
        for (int v = sink; v != source; v = parent[v]) {
            int u = parent[v];
            pathFlow = std::min(pathFlow, residual[u][v]);
        }

        
        for (int v = sink; v != source; v = parent[v]) {
            int u = parent[v];
            residual[u][v] -= pathFlow;
            residual[v][u] += pathFlow;
        }

        maxFlow += pathFlow;
    }

    return maxFlow;
}

std::vector<int> Web::findShortestPath(int startCSId, int endCSId, const std::unordered_map<int, Pipe>& pipes) const {
    
    std::unordered_map<int, double> dist;
    std::unordered_map<int, int> parent;
    std::priority_queue<std::pair<double, int>,
        std::vector<std::pair<double, int>>,
        std::greater<std::pair<double, int>>> pq;

    
    for (const auto& conn : connections) {
        dist[conn.startCSId] = std::numeric_limits<double>::infinity();
        dist[conn.endCSId] = std::numeric_limits<double>::infinity();
    }

    dist[startCSId] = 0.0;
    pq.push({ 0.0, startCSId });
    parent[startCSId] = -1;

    while (!pq.empty()) {
        double currentDist = pq.top().first;
        int u = pq.top().second;
        pq.pop();

        if (currentDist > dist[u]) {
            continue; 
        }

        if (u == endCSId) {
            break; 
        }

        
        for (const auto& conn : connections) {
            if (conn.startCSId == u) {
                int v = conn.endCSId;
                double weight = std::numeric_limits<double>::infinity();

                auto pipeIt = pipes.find(conn.pipeId);
                if (pipeIt != pipes.end()) {
                    weight = calculatePipeWeight(pipeIt->second);
                }

                if (dist[u] + weight < dist[v]) {
                    dist[v] = dist[u] + weight;
                    parent[v] = u;
                    pq.push({ dist[v], v });
                }
            }
        }
    }

    
    std::vector<int> path;
    if (dist.find(endCSId) == dist.end() || dist[endCSId] == std::numeric_limits<double>::infinity()) {
        return path; 
    }

    for (int v = endCSId; v != -1; v = parent[v]) {
        path.push_back(v);
    }
    std::reverse(path.begin(), path.end());

    return path;
}