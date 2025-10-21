#ifndef FILEOPERATIONS_H
#define FILEOPERATIONS_H

#include "PipeManager.h"
#include "CSManager.h"

void saveToFile(const PipeManager& pipeManager, const CSManager& csManager);
void loadFromFile(PipeManager& pipeManager, CSManager& csManager);

#endif