#ifndef SEARCHMENUS_H
#define SEARCHMENUS_H

#include "PipeManager.h"
#include "CSManager.h"
#include "web.h"

void searchPipeMenu(PipeManager& pipeManager, Web& network);
void searchCSMenu(CSManager& csManager, Web& network);

#endif