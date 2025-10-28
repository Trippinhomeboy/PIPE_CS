#include "PipeManager.h"
#include "Utilities.h"
#include <iostream>
#include <string>

void PipeManager::addPipe() {
    Pipe newPipe;
    newPipe.readFromConsole();
    pipes[newPipe.getId()] = newPipe;
    std::cout << "Pipe added with ID: " << newPipe.getId() << std::endl;
}

void PipeManager::displayAllPipes() const {
    if (pipes.empty()) {
        std::cout << "No pipes available." << std::endl;
        return;
    }
    std::cout << "=== ALL PIPES ===" << std::endl;
    for (const auto& pair : pipes) {
        pair.second.writeToConsole();
    }
}

void PipeManager::displayPipes(const std::unordered_map<int, Pipe>& pipeSubset) const {
    if (pipeSubset.empty()) {
        std::cout << "No pipes matching search criteria." << std::endl;
        return;
    }
    std::cout << "=== FOUND PIPES ===" << std::endl;
    for (const auto& pair : pipeSubset) {
        pair.second.writeToConsole();
    }
}

void PipeManager::editPipe() {
    if (pipes.empty()) {
        std::cout << "No pipes available for editing." << std::endl;
        return;
    }

    displayAllPipes();
    int id = inputInRange<int>("Enter pipe ID for editing: ", 1, Pipe::nextId - 1);

    auto it = pipes.find(id);
    if (it != pipes.end()) {
        it->second.editRepairStatus();
    }
    else {
        std::cout << "Pipe with ID " << id << " not found." << std::endl;
    }
}

void PipeManager::deletePipe() {
    if (pipes.empty()) {
        std::cout << "No pipes available for deletion." << std::endl;
        return;
    }

    displayAllPipes();
    int id = inputInRange<int>("Enter pipe ID for deletion: ", 1, Pipe::nextId - 1);

    auto it = pipes.find(id);
    if (it != pipes.end()) {
        pipes.erase(it);
        std::cout << "Pipe with ID " << id << " deleted." << std::endl;
    }
    else {
        std::cout << "Pipe with ID " << id << " not found." << std::endl;
    }
}

std::unordered_map<int, Pipe> PipeManager::searchByName(const std::string& name) const {
    std::unordered_map<int, Pipe> result;
    for (const auto& pair : pipes) {
        if (pair.second.getName().find(name) != std::string::npos) {
            result[pair.first] = pair.second;
        }
    }
    return result;
}

std::unordered_map<int, Pipe> PipeManager::searchByStatus(bool status) const {
    std::unordered_map<int, Pipe> result;
    for (const auto& pair : pipes) {
        if (pair.second.getStatus() == status) {
            result[pair.first] = pair.second;
        }
    }
    return result;
}

void PipeManager::editPipeSubset(const std::unordered_map<int, Pipe>& pipeSubset) {
    for (const auto& pair : pipeSubset) {
        auto pipeIt = pipes.find(pair.first);
        if (pipeIt != pipes.end()) {
            pipeIt->second.editRepairStatus();
        }
    }
}

void PipeManager::deletePipeSubset(const std::unordered_map<int, Pipe>& pipeSubset) {
    for (const auto& pair : pipeSubset) {
        pipes.erase(pair.first);
    }
}