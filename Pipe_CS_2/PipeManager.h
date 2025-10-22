#ifndef PIPEMANAGER_H
#define PIPEMANAGER_H

#include "Pipe.h"
#include <unordered_map>

class PipeManager {
private: 
    std::unordered_map<int, Pipe> pipes;

public:
    void addPipe();
    void displayAllPipes() const; 
    void displayPipes(const std::unordered_map<int, Pipe>& pipeSubset) const;
    void editPipe();
    void deletePipe();

    std::unordered_map<int, Pipe> searchByName(const std::string& name) const;
    std::unordered_map<int, Pipe> searchByStatus(bool status) const;

    void editPipeSubset(const std::unordered_map<int, Pipe>& pipeSubset);
    void deletePipeSubset(const std::unordered_map<int, Pipe>& pipeSubset);

    const std::unordered_map<int, Pipe>& getPipes() const { return pipes; }
    std::unordered_map<int, Pipe>& getPipes() { return pipes; }
};

#endif