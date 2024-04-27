#ifndef PROCESS_H
#define PROCESS_H

#include <string>
#include <fstream>
#include <vector>
#include "MemoryManager.h"

class Process {
public:
    Process(int id, const std::string& filename, MemoryManager& memManager);
    void run();

private:
    int processID;
    std::string inputFilename;
    MemoryManager& memoryManager;
    std::vector<std::pair<char, unsigned int>> pageTable;

    void loadMemoryAccesses();
    void performMemoryAccess(const std::string& access);
    unsigned int translateAddress(unsigned int virtualAddress, bool isWrite);
};

#endif // PROCESS_H
